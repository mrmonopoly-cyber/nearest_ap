

#include "leader_alive.hpp"
#include <iostream>

using namespace nearest_ap;
using Msg_t = Bus_t::Msg_t;

LeaderAliveTask_t::LeaderAliveTask_t(
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f leader_task) noexcept:
        BaseTask_t(static_cast<TaskId>(InteractibleTask::LEADER_ALIVE)),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(leader_task) 
    {
    }

LeaderAliveTask_t::LeaderAliveTask_t(
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f&& leader_task) noexcept:
        BaseTask_t(static_cast<TaskId>(InteractibleTask::LEADER_ALIVE)),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(std::move(leader_task))
    {
    }

void LeaderAliveTask_t::run(void) noexcept
{
  Msg_t msg{};
  pb_ostream_t ostream{};
  pb_ostream_t ostream_wrapper{};
  near_ap_MessageIndex msg_index
  {
    .has_msg_type = true,
    .msg_type = near_ap_MessageType_LeaderHeartbit,
    .data_count = 2,
    .data = {0},
  };
  ostream_wrapper = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
  ostream = pb_ostream_from_buffer(reinterpret_cast<pb_byte_t*>(&msg_index.data), sizeof msg_index.data);

  if (m_internal.is_leader())
  {
    near_ap_LeaderHeartbit heartbit =
    {
      .has_id = true,
      .id = static_cast<std::uint32_t>(m_internal.user_id()),
      .has_potential = true,
      .potential = m_internal.user_potential(),
    };

    if (pb_encode(&ostream, near_ap_LeaderHeartbit_fields, &heartbit) //&&
        )
    {
      pb_encode(&ostream_wrapper, near_ap_MessageIndex_fields, &msg_index);

      std::cout 
        << "i'm leader_alive:"
        << m_internal.user_id()
        << " pot: "
        << m_internal.user_potential()
        << std::endl;
      BusStatus_t error = m_bus.Write(msg);
      if (error != BusStatus_t::Ok)
      {
        //TODO: manage failures in sending
      }
    }
    else
    {
      //TODO: manage failures in serialization
    }

    m_leader_task();
  }
}
