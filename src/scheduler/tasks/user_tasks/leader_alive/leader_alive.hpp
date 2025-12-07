#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask_t: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

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
      using LeaderTask_f = std::function<void()>;

      LeaderAliveTask_t() = delete;

      LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f leader_task) noexcept;


      LeaderAliveTask_t(
          EventWriter& pipe,
          Bus_t& bus,
          const Internal_t& internal,
          const LeaderTask_f&& leader_task) noexcept;

      void run(void) noexcept override;

    private:
      Bus_t& m_bus;
      const Internal_t& m_internal;
      const LeaderTask_f m_leader_task;
  };
};
