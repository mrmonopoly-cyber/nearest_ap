#pragma once

#include "scheduler/bus/bus.hpp"
#include <nearest_ap/scheduler/scheduler.hpp>
#include <nearest_ap/internal/internal.hpp>
#include <utility>

namespace nearest_ap {

  template<typename SpawnerType >
  class Node
  {
    public:
      using Scheduler_t = Scheduler<SpawnerType>;
      using ComputePot_f = typename Scheduler_t::ComputePot_f;
      using LeaderTask_f = typename Scheduler_t::LeaderTaks_f;

      using VirtualId_t = Internal_t::VirtualId_t;

      Node() = delete;

      explicit Node(
          SpawnerType&& spawner,
          Bus_t& bus,
          ComputePot_f&& compute_pot_f,
          LeaderTask_f&& leader_task_f
          ) noexcept :
        m_internal(std::move(compute_pot_f)),
        m_scheduler(std::move(spawner), bus, std::move(leader_task_f), m_internal)
        {
        }

      void async_start(void)
      {
        m_scheduler.spawn_tasks();
      }

    private:
      Internal_t m_internal;
      Scheduler_t m_scheduler;
  };
}
