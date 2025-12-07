#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask_t: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include <cstdint>
#include <functional>

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/base_task.hpp>
#include <nearest_ap/scheduler/tasks/user_tasks/user_task.hpp>
#include <nearest_ap/scheduler/tasks/event_queue/event_queue.hpp>

#include <project_deps.h>

namespace nearest_ap {
  class LeaderAliveTask_t : public UserTask_t
  {
    public:
      using Msg_t = Bus_t::Msg_t;
      using LeaderTask_f = std::function<void()>;

      explicit LeaderAliveTask_t() noexcept;

      LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f leader_task) :
        UserTask_t(pipe),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(leader_task)
    {
    }


      LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f&& leader_task) :
        UserTask_t(pipe),
        m_bus(bus),
        m_internal(internal),
        m_leader_task(std::move(leader_task))
    {
    }

      void run(void) noexcept override
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
    private:
      Bus_t& m_bus;
      const Internal_t& m_internal;
      const LeaderTask_f m_leader_task;
  };
};
