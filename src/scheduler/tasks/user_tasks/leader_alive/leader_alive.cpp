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
  near_ap_MessageIndexV2 msg_index = near_ap_MessageIndexV2_init_default;

  ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());

  if (m_internal.is_leader())
  {
    msg_index.which_value = near_ap_MessageIndexV2_heartbit_tag;
    msg_index.value.heartbit =
    {
      .has_id = true,
      .id = static_cast<std::uint32_t>(m_internal.user_id()),
      .has_potential = true,
      .potential = m_internal.user_potential(),
    };

    if (!pb_encode(&ostream, near_ap_MessageIndexV2_fields, &msg_index))
    {
      std::cout
        << "encode error: "
        << PB_GET_ERROR(&ostream) 
        << ", at: "
        << __FILE__ 
        << ":"
        << __LINE__
        << std::endl;
    }
    BusStatus_t error = m_bus.Write(msg);
    if (error != BusStatus_t::Ok)
    {
      std::cout
        << "write error: "
        << __FILE__ 
        << ":"
        << __LINE__
        << std::endl;
    }

    m_leader_task();
  }
}
