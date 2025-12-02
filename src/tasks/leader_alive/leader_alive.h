#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include "../base_task.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<
    typename AddressType,
    std::size_t mex_payload_size = BaseTask<AddressType>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates>
    class LeaderAliveTask : BaseTask<AddressType, mex_payload_size>
  {
    public:
      using BaseTask_t = BaseTask<AddressType, mex_payload_size>;
      using SendMex_t = typename BaseTask_t::SendMex_t;
      using VoteInfo_t = VoteInfo<default_num_nodes>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(const SendMex_t& send_f, const VoteInfo_t& vote_info) noexcept;

      TaskError_t run(void) noexcept override;
    private:
      SendMex_t& m_send_f;
      const VoteInfo_t& m_vote_info;
  };
};
