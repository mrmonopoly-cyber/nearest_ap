#include "potential_election.hpp"

#include <nearest_ap/logger/logger.hpp>

using namespace nearest_ap;
using Msg_t = Bus_t::Msg_t;

PotentialElectionTask_t::PotentialElectionTask_t(Bus_t& bus,
    Internal_t& internal) noexcept :
BaseTask_t(static_cast<TaskId>(InteractibleTask::POTENTIAL_ELECTION)),
  m_bus(bus),
  m_internal(internal)
{
}

void PotentialElectionTask_t::run(void) noexcept
{
  m_internal.compute_user_potential();

  const auto check_heartbit = !m_internal.check_heartbit();

  logger::UserLog<128> buffer;
  buffer.append_msg("potential task node : ");
  buffer.append_msg(m_internal.user_id());
  buffer.append_msg(" election_sent: ");
  buffer.append_msg(m_internal.election_sent());
  buffer.append_msg(" voted: ");
  buffer.append_msg(m_internal.voted());
  buffer.append_msg(" is_leader: ");
  buffer.append_msg(m_internal.leader());
  buffer.append_msg(" user_pot: ");
  buffer.append_msg(m_internal.user_pot());
  buffer.append_msg(" leader_pot: ");
  buffer.append_msg(m_internal.leader_pot());
  buffer.append_msg(" check_heartbit: ");
  buffer.append_msg(check_heartbit);
  static_log(logger::Level::Info, buffer);

  if (
      !m_internal.leader() &&
      ( m_internal.strong_pot() || check_heartbit )
     )
  {
    Msg_t msg{};
    pb_ostream_t ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());;
    _near_ap_MessageIndexV2 msg_index_v2 = near_ap_MessageIndexV2_init_default;

    m_internal.new_election();
  
    msg_index_v2.which_value = near_ap_MessageIndexV2_new_election_tag;
    msg_index_v2.value.new_election = 
    {
      .round = m_internal.round(),
      .id = m_internal.user_id(),
      .potential = m_internal.user_pot(),
    };


    if (!pb_encode(&ostream, near_ap_MessageIndexV2_fields, &msg_index_v2))
    {
      static_log(logger::Level::Error, "encode error: ");
      return;
    }
    {
      logger::UserLog<100> log{};
      log.append_msg("node: ");
      log.append_msg(m_internal.user_id());
      log.append_msg(", starting_new_election. user potential: ");
      log.append_msg(m_internal.user_pot());
      log.append_msg(", round: ");
      log.append_msg(m_internal.round());
      static_log(logger::Level::Warning, log);
    }

    msg.m_msg_size = ostream.bytes_written;
    BusStatus_t error = m_bus.Write(msg);
    if (error != BusStatus_t::Ok)
    {
      logger::UserLog<64> log{};
      log.append_msg("node: ");
      log.append_msg(m_internal.user_id());
      log.append_msg(", write error: ");
      log.append_msg(static_cast<int>(error));
      static_log(logger::Level::Error, log);
    }

  }
}
