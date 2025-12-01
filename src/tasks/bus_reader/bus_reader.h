#pragma once

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
      using TaskError = typename BaseTask<mex_size>::TaskError;
      using SendMex = typename BaseTask<mex_size>::SendMex;
      using RecvMex = typename BaseTask<mex_size>::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;
      using LocalPotentialInfo = LocalPotentialInfo<tollerance>;

      explicit BusReaderTask() noexcept;
      BusReaderTask(
          const SendMex send_f,
          const RecvMex recv_f,
          const LocalPotentialInfo& local_potential,
          VoteInfo& vote_info
          ) noexcept;

      TaskError run(void) override;

    private:
      SendMex m_send_f = [](BaseTask<>::BusMex&){return BaseTask<mex_size>::BusStatus::Inactive;};
      RecvMex m_recv_f = [](){while(true){}};
      VoteInfo& m_vote_info;
      LocalPotentialInfo& m_local_potential_info;
  };
};
