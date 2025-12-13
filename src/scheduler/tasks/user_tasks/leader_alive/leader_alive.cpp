#include "leader_alive.hpp"
#include <string>
#include <nearest_ap/logger/logger.hpp>

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

  char buffer[32]{};

  snprintf(buffer, sizeof(buffer), "leader_alive task node: %d", m_internal.user_id());
  static_log(logger::Level::Info, buffer);

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
      char buffer[128]{};
      snprintf(buffer, sizeof(buffer),
          "encode error: %s", PB_GET_ERROR(&ostream));
      static_log(logger::Level::Error, buffer);
    }
    msg.m_msg_size = ostream.bytes_written;
    BusStatus_t error = m_bus.Write(msg);
    if (error != BusStatus_t::Ok)
    {
      char buffer[128]{};
      snprintf(buffer, sizeof(buffer),
          "node %d, write error: %d",
          m_internal.user_id(), static_cast<int>(error));
      static_log(logger::Level::Error, buffer);
    }

    char buffer[128]{};
    snprintf(buffer, sizeof(buffer), "node %d: i'm leader with pot %d",
        m_internal.user_id(), msg_index.value.heartbit.potential);
    static_log(logger::Level::Debug, buffer);
    m_leader_task();
  }
}
