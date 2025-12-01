#pragma once

#include <cstdint>
#include <functional>

#include "../base_task.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<
    std::size_t payload_max_size = BaseTask<>::m_payload_max_size,
    uint32_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    uint32_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class PotentialElectionTask : public BaseTask<payload_max_size>
  {
    public:
      using ComputePotF = std::function<int()>;
      using TaskError = typename BaseTask<payload_max_size>::TaskError;
      using SendMex = typename BaseTask<payload_max_size>::SendMex;
      using VoteInfo = VoteInfo<default_num_nodes>;
      using LocalPotentialInfo = LocalPotentialInfo<tollerance>;

      explicit PotentialElectionTask() noexcept;

      PotentialElectionTask(const SendMex send_f) noexcept;
      PotentialElectionTask(
          const SendMex send_f,
          const ComputePotF pot_f,
          LocalPotentialInfo& pot_info,
          VoteInfo& vote_info
          ) noexcept;

      TaskError run(void) override;

    private:
      SendMex m_send_f=[](BaseTask<>::BusMex&){return BaseTask<payload_max_size>::BusStatus::Inactive;};
      ComputePotF m_compute_local_potential = [](){return 0;};
      LocalPotentialInfo& m_pot_info;
      VoteInfo& m_vote_info;
  };
};
