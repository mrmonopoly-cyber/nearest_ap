


#include "bus_reader.hpp"
#include <optional>

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
  _near_ap_MessageIndex msg_index{};
  pb_istream_t stream;

  if (!msg_raw.has_value())
  {
    return;
  }

  stream = pb_istream_from_buffer(msg_raw->m_payload.data(), msg_raw->m_payload.size());
  pb_decode(&stream, near_ap_MessageIndex_fields, &msg_index);
  stream = pb_istream_from_buffer(
      reinterpret_cast<const pb_byte_t *>(msg_index.data),
      sizeof(msg_index.data)
      );

  switch (msg_index.msg_type)
  {
    case near_ap_MessageType_NewElection:
      {
        _near_ap_NewElection new_election{};
        pb_decode(&stream, near_ap_NewElection_fields, &new_election);
        if( new_election.round > m_internal.round() )
        {
          Msg_t msg{};
          pb_ostream_t ostream{};
          _near_ap_VoteRespone election_respone{
            .has_round = true,
              .round = new_election.round,
              .has_id = true,
              .id = m_internal.user_id(),
          };

          ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
          m_internal.update_round(new_election.round);
          if (pb_encode(&ostream, near_ap_VoteRespone_fields, &election_respone))
          {
            BusStatus_t error = m_bus.Write(msg);
            if (error != BusStatus_t::Ok)
            {
              //TODO: manage failures in sending
            }
          }//TODO: manage failures in serialization

        }
      }
      break;
    case near_ap_MessageType_VoteRespone:
      {
        _near_ap_VoteRespone vote_response{};
        pb_decode(&stream, near_ap_VoteRespone_fields, &vote_response);
        if (m_internal.round() == vote_response.round)
        {
          m_internal.support();
        }
      }
      break;
    case near_ap_MessageType_LeaderHeartbit:
      {
        _near_ap_LeaderHeartbit leader_heartbit{};
        pb_decode(&stream, near_ap_LeaderHeartbit_fields, &leader_heartbit);
        m_internal.check_and_set_leader(leader_heartbit.id, leader_heartbit.potential);
      }
      break;
  }
}
