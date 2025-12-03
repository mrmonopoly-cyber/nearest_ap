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

#include "../base_task.h"
#include "../../bus/bus.h"
#include "../../../internal/internal.h"

namespace nearest_ap {
  template<typename AddressType, typename BusType >
    class PotentialElectionTask : public BaseTask_t
  {
    public:
      using Internal_t = Internal<AddressType>;

      using ComputePotF = std::function<int()>;

      explicit PotentialElectionTask() = delete;

      PotentialElectionTask(
          const BusType& bus,
          const ComputePotF& pot_f,
          Internal_t& internal) noexcept :
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
