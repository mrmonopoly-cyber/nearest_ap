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
#include "../bus/bus.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<typename AddressType, std::size_t payload_max_size = Bus<AddressType>::m_payload_max_size >
    class PotentialElectionTask : public BaseTask_t
  {
    public:
      using Bus_t = Bus<AddressType, payload_max_size>;

      using ComputePotF = std::function<int()>;

      explicit PotentialElectionTask() = delete;

      PotentialElectionTask(
          const Bus_t& bus,
          const ComputePotF& pot_f,
          LocalPotentialInfo_t& pot_info,
          VoteInfo_t& vote_info
          ) noexcept :
        m_bus(bus),
        m_compute_local_potential(pot_f),
        m_pot_info(pot_info),
        m_vote_info(vote_info)
        {
        }

      TaskError_t run(void)override
      {
        return TaskError_t::Error;
      }


    private:
      const Bus_t& m_bus;
      const ComputePotF& m_compute_local_potential;
      LocalPotentialInfo_t& m_pot_info;
      VoteInfo_t& m_vote_info;
  };
};
