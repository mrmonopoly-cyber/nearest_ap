#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include "../base_task.h"
#include "../../internal/internal.h"
#include <iterator>

namespace nearest_ap {
  template<
    std::size_t mex_payload_size = BaseTask<>::m_payload_max_size,
    std::size_t default_num_nodes=VoteInfo<>::m_default_num_candidates>
    class LeaderAliveTask : BaseTask<mex_payload_size>
  {
    public:
      using BaseTaskLeader_t = BaseTask<mex_payload_size>;
      using TaskError = typename BaseTaskLeader_t::TaskError;
      using SendMex = typename BaseTaskLeader_t::SendMex;
      using VoteInfo_t = VoteInfo<default_num_nodes>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(const SendMex& send_f, const VoteInfo_t& vote_info) noexcept;

      TaskError run(void) noexcept override;
    private:
      SendMex& m_send_f;
      const VoteInfo_t& m_vote_info;
  };
};
