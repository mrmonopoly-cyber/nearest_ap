#pragma once

#include <nearest_ap/scheduler/scheduler.hpp>
#include <nearest_ap/internal/internal.hpp>

namespace nearest_ap {

  template<typename BusType, typename SpawnerType >
  class Node
  {
    public:
      using Scheduler_t = Scheduler< BusType, SpawnerType>;
      using AddressType_t = Internal_t::AddressType_t;
      using ComputePot_f = typename Scheduler_t::ComputePot_f;

      Node() = delete;

      explicit Node(
          SpawnerType&& spawner,
          BusType&& bus,
          ComputePot_f&& compute_pot_f) noexcept :
        m_internal(std::move(compute_pot_f)),
        m_scheduler(std::move(spawner), std::move(bus), m_internal)
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
