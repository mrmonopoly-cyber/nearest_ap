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

#include "../base_task.h"
#include "../../internal/internal.h"
#include <cstddef>
#include <cstdint>

namespace nearest_ap {
  template<std::size_t mex_size = BaseTask<>::m_payload_max_size,
    uint32_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    uint32_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class BusReaderTask : public BaseTask<mex_size>
  {
    public:
      using BaseTask_t = BaseTask<mex_size>;
      using TaskError = typename BaseTask_t::TaskError;
      using SendMex = typename BaseTask_t::SendMex;
      using RecvMex = typename BaseTask_t::SendMex;
      using VoteInfo_t = VoteInfo<default_num_nodes>;
      using LocalPotentialInfo_t = LocalPotentialInfo<tollerance>;

      explicit BusReaderTask() noexcept;
      BusReaderTask(
          const SendMex& send_f,
          const RecvMex& recv_f,
          const LocalPotentialInfo_t& local_potential,
          VoteInfo_t& vote_info
          ) noexcept;

      TaskError run(void) noexcept override;

    private:
      SendMex& m_send_f = [](BaseTask<>::BusMex&){return BaseTask_t::BusStatus::Inactive;};
      RecvMex& m_recv_f = [](){while(true){}};
      VoteInfo_t& m_vote_info;
      LocalPotentialInfo_t & m_local_potential_info;
  };
};
