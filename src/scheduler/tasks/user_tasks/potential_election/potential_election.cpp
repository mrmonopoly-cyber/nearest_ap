#include "potential_election.hpp"

#include <cstdint>
#include <nearest_ap/logger/logger.hpp>

using namespace nearest_ap;
using Msg_t = Bus_t::Msg_t;

PotentialElectionTask_t::PotentialElectionTask_t(
    Bus_t& bus,
    Internal_t& internal) noexcept :
BaseTask_t(static_cast<TaskId>(InteractibleTask::POTENTIAL_ELECTION),s_base_freq),
  m_bus(bus),
  m_internal(internal)
{
}

PotentialElectionTask_t::PotentialElectionTask_t(
    Bus_t& bus,
    Internal_t& internal,
    const Millis_t freq ) noexcept :
BaseTask_t(static_cast<TaskId>(InteractibleTask::POTENTIAL_ELECTION),freq),
  m_bus(bus),
  m_internal(internal)
{
}

void PotentialElectionTask_t::run(void) noexcept
{
  m_internal.compute_user_potential();

  logger::UserLog<200> buffer;
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
  buffer.append_msg(" best candidate: ");
  buffer.append_msg(m_internal.better_candidate());
  buffer.append_msg(" best candidate pot: ");
  buffer.append_msg(m_internal.better_candidate_pot());
  static_log(logger::Level::Info, buffer);

  if (m_internal.user_valid_for_election() )
  {
    Msg_t msg{};
    pb_ostream_t ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());;
    _near_ap_MessageIndexV2 msg_index_v2 = near_ap_MessageIndexV2_init_default;

    if (!m_internal.is_best_candidate())
    {
      logger::UserLog<128>log{};
      log.append_msg("node: ");
      log.append_msg(m_internal.user_id());
      log.append_msg("increasing time_scale_factor: ");
      log.append_msg(++m_internal.m_potential_election_time_scale);
      log.append_msg(", current potential tasks freq: ");
      log.append_msg(static_cast<uint32_t>(freq()));
      static_log(logger::Level::Warning, log);
      update_freq_by_factor(m_internal.m_potential_election_time_scale.get());
    }

    m_internal.new_election();
  
    msg_index_v2.which_value = near_ap_MessageIndexV2_new_election_tag;
    msg_index_v2.value.new_election.round = m_internal.round();
    msg_index_v2.value.new_election.round = m_internal.round();
    msg_index_v2.value.new_election.id = m_internal.user_id();
    msg_index_v2.value.new_election.potential = m_internal.user_pot();


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
      log.append_msg(static_cast<std::uint32_t>(error));
      static_log(logger::Level::Error, log);
    }

  }
}
