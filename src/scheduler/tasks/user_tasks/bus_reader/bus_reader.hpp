#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * BusReaderTask_t: receive messages from the bus and update internal data
 *
 * Possible messages:
 *
 * 1- NewElection (round, id, potential):
 *            IF (round > local round) AND (potential > local potential) THEN
 *              vote.update_round(round);
 *              SEND ElectionVote(round, id)
 * 2- ElectionVote (round, id):
 *            IF round == current election round THEN
 *              vote.increment();
 * 3- LeaderAlive (id, potential): 
 *              IF potential > local leader potential THEN
 *              vote.update_leader(id);
 *              potential_info.update_leader_pot(potential);
 */

#include <project_deps.h>

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/base_task.hpp>

namespace nearest_ap {
  class BusReaderTask_t : public BaseTask_t
  {
    public:
      BusReaderTask_t() = delete;
      BusReaderTask_t(Bus_t& bus, Internal_t& internal) noexcept;

      void run(void) noexcept override;

    private:
      Bus_t& m_bus;
      Internal_t& m_internal;
  };
};
