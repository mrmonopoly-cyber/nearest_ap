#include "leader_alive.hpp"
#include <cstdint>
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

  logger::UserLog<32>log{};
  log.append_msg("leader_alive task node: ");
  log.append_msg(m_internal.user_id());
  static_log(logger::Level::Info, log);

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
      logger::UserLog<48>log{};
      log.append_msg("encode error: ");
      log.append_msg(PB_GET_ERROR(&ostream));
      static_log(logger::Level::Error, log);
    }
    msg.m_msg_size = ostream.bytes_written;
    BusStatus_t error = m_bus.Write(msg);
    if (error != BusStatus_t::Ok)
    {
      logger::UserLog<48>log{};
      log.append_msg("node ");
      log.append_msg(m_internal.user_id());
      log.append_msg(", write error: ");
      log.append_msg(static_cast<std::int32_t>(error));
      static_log(logger::Level::Error, log);
    }

    logger::UserLog<48>log{};
    log.append_msg("node ");
    log.append_msg(m_internal.user_id());
    log.append_msg(": i'm leader with pot ");
    log.append_msg(msg_index.value.heartbit.potential);
    static_log(logger::Level::Debug, log);
    m_leader_task();
  }
}
