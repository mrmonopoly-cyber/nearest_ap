#include "bus_reader.hpp"

#include <optional>
#include <nearest_ap/logger/logger.hpp>
#include <stdio.h>

using namespace nearest_ap;

using Msg_t = typename Bus_t::Msg_t;

BusReaderTask_t::BusReaderTask_t(
    Bus_t& bus,
    Internal_t& internal) noexcept:
BaseTask_t(static_cast<TaskId>(InteractibleTask::BUS_READER)),
  m_bus(bus),
  m_internal(internal)
{
}

void BusReaderTask_t::run(void) noexcept 
{
  std::optional<Msg_t> msg_raw{m_bus.Read()};
  _near_ap_MessageIndexV2 msg_index{};
  pb_istream_t stream;
  char buffer[256]{};

  snprintf(buffer, sizeof(buffer), "bus reader task node: %d", m_internal.user_id());
  static_log(logger::Level::Info, buffer);

  if (!msg_raw.has_value())
  {
    return;
  }

  stream = pb_istream_from_buffer(msg_raw->m_payload.data(), msg_raw->m_msg_size);
  if(!pb_decode(&stream, near_ap_MessageIndexV2_fields, &msg_index))
  {
    char buffer[128]{};
    snprintf(buffer, sizeof(buffer), "node: %d, decode error: %s, tag: %d", 
        id(), PB_GET_ERROR(&stream), msg_index.which_value);
    static_log(logger::Level::Error, buffer);
    return;
  }

  switch (msg_index.which_value)
  {
    case near_ap_MessageIndexV2_heartbit_tag:
      {
        const auto new_leader_id = msg_index.value.heartbit.id;
        const auto new_leader_pot = msg_index.value.heartbit.potential;

        m_internal.check_and_set_leader(new_leader_id, new_leader_pot);
        snprintf(buffer, sizeof(buffer),
            "current node: %d, recevied leader_heartbit: %d:%d",
            m_internal.user_id(), new_leader_id, new_leader_pot);
        static_log(logger::Level::Debug, buffer);
      }
      break;
    case near_ap_MessageIndexV2_new_election_tag:
      {
        const auto new_round = msg_index.value.new_election.round;
        const auto new_pot = msg_index.value.new_election.potential;
        const auto new_leader = msg_index.value.new_election.id;

        snprintf(buffer, sizeof(buffer),
            "current node: %d, recevied new election request. new_leader:%d -- round:%d -- pot:%d",
            m_internal.user_id(), new_leader, new_round, new_pot);

        static_log(logger::Level::Info, buffer);
        if ( new_round > m_internal.round() && new_pot > m_internal.user_potential())
        {
          pb_ostream_t ostream = pb_ostream_from_buffer(
              msg_raw->m_payload.data(),
              msg_raw->m_payload.size());

          m_internal.update_round(new_round);
          msg_index.which_value = near_ap_MessageIndexV2_vote_response_tag,
          msg_index.value.vote_response = 
          {
            .has_round = true,
            .round = new_round,
            .has_supporter = true,
            .supporter = m_internal.user_id(),
            .has_new_leader = true,
            .new_leader = new_leader,
          };

          if (!pb_encode(&ostream, near_ap_MessageIndexV2_fields, &msg_index))
          {
            char buffer[128]{};
            snprintf(buffer, sizeof(buffer), "encode error: %s", PB_GET_ERROR(&ostream));
            static_log(logger::Level::Error, buffer);
          }
          msg_raw->m_msg_size = ostream.bytes_written;
          BusStatus_t error = m_bus.Write(*msg_raw);
          if (error != BusStatus_t::Ok)
          {
            static_log(logger::Level::Error, "write error");
          }
        }
      }
      break;
    case near_ap_MessageIndexV2_vote_response_tag:
      {
        const auto user_vote_id = msg_index.value.vote_response.new_leader;
        const auto election_round = msg_index.value.vote_response.round;

        snprintf(buffer, sizeof(buffer),
            "current node: %d, recevied new vote response. :supporter%d -- round:%d",
            m_internal.user_id(), user_vote_id, election_round);
        static_log(logger::Level::Debug, buffer);

        if (election_round == m_internal.round() &&
            m_internal.in_election() && 
            user_vote_id == m_internal.user_id())
        {
          m_internal.support();
        }
      }
      break;
  }
}
