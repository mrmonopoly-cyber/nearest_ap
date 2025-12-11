#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * PotentialElectionTask_t: every K time:
 *          update local potential = compute_potential();
 *          IF leader potential + tollerance < local potential AND vote.no_new_election() THEN:
 *              local round = vote.start_new_election();
 *              SEND NewElection(round, local id, local potential);
 */

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/base_task.hpp>

#include <project_deps.h>

namespace nearest_ap {
  class PotentialElectionTask_t : public BaseTask_t
  {
    public:
      PotentialElectionTask_t() = delete;
      PotentialElectionTask_t(Bus_t& bus, Internal_t& internal) noexcept;

      void run(void) noexcept override;

    private:
      Bus_t& m_bus;
      Internal_t& m_internal;
  };
};
