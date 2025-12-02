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
  template<
    typename AddressType,
    std::size_t payload_max_size = Bus<AddressType>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class PotentialElectionTask : public BaseTask_t
  {
    public:
      using LocalPotentialInfo_t = LocalPotentialInfo<tollerance>;
      using VoteInfo_t = VoteInfo<default_num_nodes>;
      using Bus_t = Bus<AddressType, payload_max_size>;

      using ComputePotF = std::function<int()>;

      explicit PotentialElectionTask() noexcept;

      PotentialElectionTask(Bus_t& bus) noexcept;
      PotentialElectionTask(
          const Bus_t& bus,
          const ComputePotF pot_f,
          LocalPotentialInfo_t& pot_info,
          VoteInfo_t& vote_info
          ) noexcept;

      TaskError_t run(void) override;

    private:
      Bus_t& m_bus;
      ComputePotF m_compute_local_potential = [](){return 0;};
      LocalPotentialInfo_t& m_pot_info;
      VoteInfo_t& m_vote_info;
  };
};
