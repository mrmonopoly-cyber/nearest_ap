#pragma once

#include "../base_task.h"
#include "../../internal/internal.h"
#include <iterator>

namespace nearest_ap {
  template<
    std::size_t mex_payload_size = BaseTask<>::m_payload_max_size,
    uint32_t default_num_nodes=VoteInfo<>::m_default_num_candidates>
    class LeaderAliveTask : BaseTask<mex_payload_size>
  {
    public:
      using TaskError = typename BaseTask<mex_payload_size>::TaskError;
      using SendMex = typename BaseTask<mex_payload_size>::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(const SendMex send_f, const VoteInfo& vote_info) noexcept;

      TaskError run(void) override;
    private:
      SendMex m_send_f;
      const VoteInfo& m_vote_info;
  };
};
