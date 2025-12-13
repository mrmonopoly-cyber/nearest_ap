#include "bus_reader.hpp"

#include <optional>
#include <string>
#include <nearest_ap/logger/logger.hpp>

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

  if (!msg_raw.has_value())
  {
    return;
  }

  stream = pb_istream_from_buffer(msg_raw->m_payload.data(), msg_raw->m_payload.size());
  pb_decode(&stream, near_ap_MessageIndexV2_fields, &msg_index);

  switch (msg_index.which_value)
  {
    case near_ap_MessageIndexV2_heartbit_tag:
      {
        const auto new_leader_id = msg_index.value.heartbit.id;
        const auto new_leader_pot = msg_index.value.heartbit.potential;
        char buffer[256]{};

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
            .has_id = true,
            .id = m_internal.user_id()
          };

          if (!pb_encode(&ostream, near_ap_MessageIndexV2_fields, &msg_index))
          {
            char buffer[128]{};
            snprintf(buffer, sizeof(buffer), "encode error: %s", PB_GET_ERROR(&ostream));
            static_log(logger::Level::Error, buffer);
          }
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
        const auto user_vote_id = msg_index.value.vote_response.id;
        (void) user_vote_id;
        const auto election_round = msg_index.value.vote_response.round;

        //TODO: add check on vote id

        if (election_round == m_internal.round() && m_internal.in_election())
        {
          m_internal.support();
        }
      }
      break;
  }
}
