#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * BusReaderTask: receive messages from the bus and update internal data
 *
 * Possible messages:
 *
 * 1- LeaderAlive (potential): 
 *            potential_info.update_leader_pot(potential);
 * 2- NewElection (round, id, potential):
 *            IF (round > local round) AND (potential > local potential) THEN
 *              vote.update_round(round);
 *              SEND ElectionVote(round, id)
 * 3- ElectionVote (round, id):
 *            IF round == current election round THEN
 *              vote.increment();
 */

#include "../user_task.hpp"

#include "../../../../internal/internal.hpp"
#include "../../../bus/bus.hpp"

namespace nearest_ap {
  template<typename AddressType, typename BusType >
    class BusReaderTask : public UserTask_t
  {
    public:
      using Internal_t = Internal<AddressType>;

      explicit BusReaderTask() = delete;

      BusReaderTask(
          EventWriter& m_pipe,
          BusType& bus,
          Internal_t& internal) noexcept:
        UserTask_t(m_pipe),
        m_bus(bus),
        m_internal(internal)
        {
        }

      void run(void) noexcept override
      {
      }

    private:
      BusType& m_bus;
      Internal_t& m_internal;
  };
};
