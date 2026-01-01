#include "bus_reader.hpp"

#include <optional>
#include <nearest_ap/logger/logger.hpp>

using namespace nearest_ap;

using Msg_t = typename Bus_t::Msg_t;

BusReaderTask_t::BusReaderTask_t(
    Bus_t& bus,
    Internal_t& internal) noexcept:
BaseTask_t(static_cast<TaskId>(InteractibleTask::BUS_READER), s_base_freq),
  m_bus(bus),
  m_internal(internal)
{
}

BusReaderTask_t::BusReaderTask_t(
    Bus_t& bus,
    Internal_t& internal,
    const Millis_t freq) noexcept:
BaseTask_t(static_cast<TaskId>(InteractibleTask::BUS_READER), freq),
  m_bus(bus),
  m_internal(internal)
{
}

void BusReaderTask_t::run(void) noexcept 
{
  std::optional<Msg_t> msg_raw{m_bus.Read()};
  _near_ap_MessageIndexV2 msg_index{};
  pb_istream_t stream;
  logger::UserLog<200>log{};
  
  if (!msg_raw.has_value())
  {
    return;
  }

  stream = pb_istream_from_buffer(msg_raw->m_payload.data(), msg_raw->m_msg_size);
  if(!pb_decode(&stream, near_ap_MessageIndexV2_fields, &msg_index))
  {
    logger::UserLog<128>log{};
    log.append_msg("node: ");
    log.append_msg(m_internal.user_id());
    log.append_msg(", decode error: ");
    log.append_msg(PB_GET_ERROR(&stream));
    static_log(logger::Level::Error, log);
    return;
  }

  switch (msg_index.which_value)
  {
    case near_ap_MessageIndexV2_heartbit_tag:
      {
        const auto new_leader_id = msg_index.value.heartbit.id;
        const auto new_leader_pot = msg_index.value.heartbit.potential;
        const auto new_leader_round = msg_index.value.heartbit.round;

        if (m_internal.m_potential_election_time_scale.get()>0)
        {
          --m_internal.m_potential_election_time_scale;
        }

        if (m_internal.leader())
        {
          log.append_msg("current node: ");
          log.append_msg(m_internal.user_id());
          log.append_msg(", recevied new NON CHECKED leader_heartbit: ");
          log.append_msg("id:");
          log.append_msg(new_leader_id);
          log.append_msg(", pot: ");
          log.append_msg(new_leader_pot);
          log.append_msg(", round: ");
          log.append_msg(new_leader_round);
          log.append_msg("| internal>");
          log.append_msg(" round: ");
          log.append_msg(m_internal.round());
          static_log(logger::Level::Debug, log);
          log.reset();
        }

        m_internal.recv_heartbit_best_candidate(new_leader_id, new_leader_pot);

        if(m_internal.recv_heartbit(new_leader_id, new_leader_pot, new_leader_round))
        {
          if (m_internal.leader())
          {
            log.append_msg("current node: ");
            log.append_msg(m_internal.user_id());
            log.append_msg(", recevied valid leader_heartbit: ");
            log.append_msg("id:");
            log.append_msg(new_leader_id);
            log.append_msg(", pot: ");
            log.append_msg(new_leader_pot);
            log.append_msg(", round: ");
            log.append_msg(new_leader_round);
            log.append_msg("| internal>");
            log.append_msg(" round: ");
            log.append_msg(m_internal.round());
            static_log(logger::Level::Debug, log);
          }
        }
      }
      break;
    case near_ap_MessageIndexV2_new_election_tag:
      {
        const auto new_round = msg_index.value.new_election.round;
        const auto new_pot = msg_index.value.new_election.potential;
        const auto new_leader = msg_index.value.new_election.id;

        if(new_round >= m_internal.round())
        {
          m_internal.update_round(new_round);
        }

        m_internal.recv_heartbit_best_candidate(new_leader,new_pot);

        if (
            new_pot > m_internal.user_pot() &&
            new_pot >= m_internal.better_candidate_pot()
           )
        {

          log.append_msg("current node: ");
          log.append_msg(m_internal.user_id());
          log.append_msg(", recevied new election request. new_leader:");
          log.append_msg(new_leader);
          log.append_msg(" -- round:");
          log.append_msg(new_round);
          log.append_msg(" -- pot");
          log.append_msg(new_pot);
          log.append_msg(". Internal: ");
          log.append_msg(" round: ");
          log.append_msg(m_internal.round());
          log.append_msg(" user_id: ");
          log.append_msg(m_internal.user_id());
          log.append_msg(" local pot: ");
          log.append_msg(m_internal.user_pot());
          log.append_msg(" better candidate: ");
          log.append_msg(m_internal.better_candidate());
          log.append_msg(" better candidate pot: ");
          log.append_msg(m_internal.better_candidate_pot());
          static_log(logger::Level::Info, log);

          pb_ostream_t ostream = pb_ostream_from_buffer(
              msg_raw->m_payload.data(),
              msg_raw->m_payload.size());

          msg_raw->reset();
          m_internal.abort_election(new_round, new_leader, new_pot);

          msg_index.which_value = near_ap_MessageIndexV2_vote_response_tag,
          msg_index.value.vote_response = 
          {
            .round = new_round,
            .potential = new_pot,
            .new_leader = new_leader,
          };

          static_log(logger::Level::Info, log);

          if (!pb_encode(&ostream, near_ap_MessageIndexV2_fields, &msg_index))
          {
            logger::UserLog<32>log{};
            log.append_msg("encode error: ");
            log.append_msg(PB_GET_ERROR(&ostream));
            static_log(logger::Level::Error, log);
            break;
          }
          msg_raw->m_msg_size = ostream.bytes_written;
          BusStatus_t error = m_bus.Write(*msg_raw);
          if (error != BusStatus_t::Ok)
          {
            static_log(logger::Level::Error, "write error");
            break;
          }
        }
      }
      break;
    case near_ap_MessageIndexV2_vote_response_tag:
      {
        const auto leader_id = msg_index.value.vote_response.new_leader;
        const auto election_round = msg_index.value.vote_response.round;

        if(election_round  >= m_internal.round())
        {
          m_internal.update_round(election_round);
        }

        m_internal.recv_heartbit_best_candidate(leader_id, msg_index.value.vote_response.potential);

        m_internal.maybe_new_best_candidate(leader_id, msg_index.value.vote_response.potential);

        if (
            m_internal.election_sent() &&
            election_round == m_internal.round() &&
            leader_id == m_internal.user_id()
           )
        {
          log.reset();
          log.append_msg("current node: ");
          log.append_msg(m_internal.user_id());
          log.append_msg(", accepted new vote response. supporter: ");
          log.append_msg(" round: ");
          log.append_msg(msg_index.value.vote_response.round);
          log.append_msg(" new_leader: ");
          log.append_msg(msg_index.value.vote_response.new_leader);
          log.append_msg(" potential: ");
          log.append_msg(msg_index.value.vote_response.potential);
          log.append_msg(". Internal: ");
          log.append_msg(" round: ");
          log.append_msg(m_internal.round());
          log.append_msg(" user_id: ");
          log.append_msg(m_internal.user_id());

          static_log(logger::Level::Debug, log);

          m_internal.support_check_wining();
        }
      }
      break;
  }
}
