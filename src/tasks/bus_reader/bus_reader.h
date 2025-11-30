#pragma once

#include "../base_task.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t default_num_nodes>
    class BusReaderTask : public BaseTask<BusMex>
  {
    public:
      using TaskError = typename BaseTask<BusMex>::TaskError;
      using SendMex = typename BaseTask<BusMex>::SendMex;
      using RecvMex = typename BaseTask<BusMex>::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;

      explicit BusReaderTask() noexcept;
      BusReaderTask(
          const SendMex send_f,
          const RecvMex recv_f,
          const LocalPotentialInfo& local_potential,
          VoteInfo& vote_info
          ) noexcept;

      TaskError run(void) override;

    private:
      SendMex m_send_f = [](BusMex&){return BaseTask<BusMex>::BusStatus::Inactive;};
      RecvMex m_recv_f = [](){while(true){}};
      VoteInfo& m_vote_info;
      const LocalPotentialInfo& m_local_potential_info;
  };
};
