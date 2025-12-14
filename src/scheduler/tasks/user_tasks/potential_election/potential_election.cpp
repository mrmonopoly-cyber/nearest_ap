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

  logger::UserLog<64> buffer;
  buffer.append_msg("potential task node: ");
  buffer.append_msg(m_internal.user_id());
  static_log(logger::Level::Info, buffer);

  const auto no_hearthbit = m_internal.consume_heartbit();
  const auto user_better_pot = m_internal.user_pot_better_leader_pot();
  if (!m_internal.is_leader() &&
      !m_internal.in_election() &&
      (user_better_pot || !no_hearthbit))
  {
    Msg_t msg{};
    pb_ostream_t ostream{};
    _near_ap_MessageIndexV2 msg_index_v2 = near_ap_MessageIndexV2_init_default;

    msg_index_v2.which_value = near_ap_MessageIndexV2_new_election_tag;
    msg_index_v2.value.new_election = 
    {
      .has_round = true,
      .round = m_internal.round(),
      .has_id = true,
      .id = m_internal.user_id(),
      .has_potential = true,
      .potential = m_internal.user_potential(),
    };


    ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());

    if (!pb_encode(&ostream, near_ap_NewElection_fields, &msg_index_v2))
    {
      static_log(logger::Level::Error, "encode error: ");
      return;
    }
    {
      logger::UserLog<100> log{};
      log.append_msg("node: ");
      log.append_msg(m_internal.user_id());
      log.append_msg(", starting_new_election. user_pot_case: ");
      log.append_msg(user_better_pot);
      log.append_msg(", no_hearthbit: ");
      log.append_msg(no_hearthbit);
      static_log(logger::Level::Warning, log);
    }

    m_internal.new_election();
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
