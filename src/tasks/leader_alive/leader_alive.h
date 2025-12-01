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
      using BaseTaskLeaderAlive = BaseTask<mex_payload_size>;
      using TaskError = typename BaseTaskLeaderAlive::TaskError;
      using SendMex = typename BaseTaskLeaderAlive::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(const SendMex& send_f, const VoteInfo& vote_info) noexcept;

      TaskError run(void) noexcept override;
    private:
      SendMex& m_send_f;
      const VoteInfo& m_vote_info;
  };
};
