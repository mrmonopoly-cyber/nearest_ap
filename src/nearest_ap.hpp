#pragma once

#include "scheduler/scheduler.hpp"
#include "internal/internal.hpp"

namespace nearest_ap {

  template<typename AddressType, typename BusType, typename SpawnerType >
  class Node
  {
    public:
      using Scheduler_t = Scheduler<AddressType, BusType, SpawnerType>;
      using Internal_t = Internal<AddressType>;
      using VoteInfo_t = VoteInfo<AddressType>;

      using ComputePotF = typename Scheduler_t::ComputePotF;

      Node() = delete;

      explicit Node(
          SpawnerType&& spawner,
          BusType&& bus,
          ComputePotF&& compute_pot_f) noexcept :
        m_internal(),
        m_scheduler(std::move(spawner), std::move(bus), std::move(compute_pot_f), m_internal)
        {
        }

      void async_start(void)
      {
        m_scheduler.spawn_tasks();
      }

      void update_id(AddressType id)
      {
        m_internal.m_current_user = id;
      }

    private:
      Internal_t m_internal;
      Scheduler_t m_scheduler;
  };
}
