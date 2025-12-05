#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * PotentialElectionTask: every K time:
 *          update local potential = compute_potential();
 *          IF leader potential + tollerance < local potential AND vote.no_new_election() THEN:
 *              local round = vote.start_new_election();
 *              SEND NewElection(round, local id, local potential);
 */

#include <functional>

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/user_tasks/user_task.hpp>
#include <nearest_ap/scheduler/tasks/event_queue/event_queue.hpp>

namespace nearest_ap {
  template< typename BusType >
    class PotentialElectionTask : public UserTask_t
  {
    public:
      using ComputePotF = std::function<int()>;

      explicit PotentialElectionTask() = delete;

      PotentialElectionTask(
          EventWriter& pipe,
          const BusType& bus,
          const ComputePotF& pot_f,
          Internal_t& internal) noexcept :
        UserTask_t(pipe),
        m_bus(bus),
        m_compute_local_potential(pot_f),
        m_internal(internal)
        {
        }

      void run(void)override
      {
      }

    private:
      const BusType& m_bus;
      const ComputePotF& m_compute_local_potential;
      Internal_t& m_internal;
  };
};
