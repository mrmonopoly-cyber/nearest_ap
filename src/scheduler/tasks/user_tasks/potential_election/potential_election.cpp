

#include "potential_election.hpp"

using namespace nearest_ap;
using Msg_t = Bus_t::Msg_t;

PotentialElectionTask_t::PotentialElectionTask_t(EventWriter& pipe, Bus_t& bus,
    Internal_t& internal) noexcept :
UserTask_t(pipe),
  m_bus(bus),
  m_internal(internal)
{
}

void PotentialElectionTask_t::run(void) noexcept
{
  m_internal.compute_user_potential();
  if (m_internal.user_pot_better_leader_pot()) //TODO: add tollerance
  {
    Msg_t msg{};
    pb_ostream_t ostream{};
    near_ap_NewElection new_election{
      .has_round = true,
        .round = m_internal.round(),
        .has_id = true,
        .id = m_internal.user_id(),
        .has_potential = true,
        .potential = m_internal.user_potential(),
    };

    ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());

    if (pb_encode(&ostream, near_ap_NewElection_fields, &new_election))
    {
      BusStatus_t error = m_bus.Write(msg);
      if (error != BusStatus_t::Ok)
      {
        //TODO: manage failures in sending
      }
    }//TODO: manage failures in serialization

    m_internal.new_election();
  }
}
