#pragma once

#include "../base_task.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t default_num_nodes>
    class LeaderAliveTask : BaseTask<BusMex>
  {
    public:
      using TaskError = typename BaseTask<BusMex>::TaskError;
      using SendMex = typename BaseTask<BusMex>::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(const SendMex send_f, const VoteInfo& vote_info) noexcept;

      TaskError run(void) override;
    private:
      SendMex m_send_f;
      const VoteInfo& m_vote_info;
  };
};
