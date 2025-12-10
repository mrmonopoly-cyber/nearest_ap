

#include "leader_alive.hpp"

using namespace nearest_ap;
using Msg_t = Bus_t::Msg_t;

LeaderAliveTask_t::LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f leader_task) noexcept:
        UserTask_t(static_cast<TaskId>(InteractibleTask::LEADER_ALIVE), pipe),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(leader_task) 
    {
    }

LeaderAliveTask_t::LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f&& leader_task) noexcept:
        UserTask_t(static_cast<TaskId>(InteractibleTask::LEADER_ALIVE), pipe),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(std::move(leader_task))
    {
    }

void LeaderAliveTask_t::run(void) noexcept
{
  Msg_t msg{};
  pb_ostream_t ostream{};

  ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
  if (m_internal.is_leader())
  {
    near_ap_LeaderHeartbit heartbit =
    {
      .has_id = true,
      .id = static_cast<std::uint32_t>(m_internal.user_id()),
      .has_potential = true,
      .potential = m_internal.user_potential(),
    };

    if (pb_encode(&ostream, near_ap_LeaderHeartbit_fields, &heartbit))
    {
      BusStatus_t error = m_bus.Write(msg);
      if (error != BusStatus_t::Ok)
      {
        //TODO: manage failures in sending
      }
    }//TODO: manage failures in serialization

    m_leader_task();
  }
}
