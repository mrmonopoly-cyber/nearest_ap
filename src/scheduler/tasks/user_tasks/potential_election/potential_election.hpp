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

#include "../../event_queue/event_queue.hpp"
#include "../user_task.hpp"
#include "../../../bus/bus.hpp"
#include "../../../../internal/internal.hpp"

namespace nearest_ap {
  template<typename AddressType, typename BusType >
    class PotentialElectionTask : public UserTask_t
  {
    public:
      using Internal_t = Internal<AddressType>;

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
