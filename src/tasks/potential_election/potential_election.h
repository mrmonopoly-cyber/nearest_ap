#pragma once

#include <cstdint>
#include <functional>

#include "../base_task.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t default_num_nodes, uint32_t tollerance>
    class PotentialElectionTask : public BaseTask<BusMex>
  {
    public:
      using ComputePotF = std::function<int()>;
      using TaskError = typename BaseTask<BusMex>::TaskError;
      using SendMex = typename BaseTask<BusMex>::SendMex;
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
      SendMex m_send_f=[](BusMex&){return BaseTask<BusMex>::BusStatus::Inactive;};
      ComputePotF m_compute_local_potential = [](){return 0;};
      LocalPotentialInfo& m_pot_info;
      VoteInfo& m_vote_info;
  };
};
