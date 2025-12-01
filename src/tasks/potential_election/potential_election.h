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
#include "../../internal/internal.h"

namespace nearest_ap {
  template<
    std::size_t payload_max_size = BaseTask<>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class PotentialElectionTask : public BaseTask<payload_max_size>
  {
    public:
      using BaseTask_t = BaseTask<payload_max_size>;
      using ComputePotF = std::function<int()>;
      using TaskError = typename BaseTask_t::TaskError;
      using SendMex = typename BaseTask_t::SendMex;
      using LocalPotentialInfo_t = LocalPotentialInfo<tollerance>;
      using VoteInfo_t = VoteInfo<default_num_nodes>;

      explicit PotentialElectionTask() noexcept;

      PotentialElectionTask(const SendMex send_f) noexcept;
      PotentialElectionTask(
          const SendMex& send_f,
          const ComputePotF pot_f,
          LocalPotentialInfo_t& pot_info,
          VoteInfo_t& vote_info
          ) noexcept;

      TaskError run(void) override;

    private:
      SendMex& m_send_f=[](BaseTask<>::BusMex&){return BaseTask_t::BusStatus::Inactive;};
      ComputePotF m_compute_local_potential = [](){return 0;};
      LocalPotentialInfo_t& m_pot_info;
      VoteInfo_t& m_vote_info;
  };
};
