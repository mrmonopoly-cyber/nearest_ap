#pragma once

#include <functional>

#include "../base_task.h"
#include "../../internal/potential_info/potential_info.h"

template<typename BusMex>
class PotentialElectionTask : public BaseTask<BusMex>
{
  public:
    using ComputePotF = std::function<int()>;
    using TaskError = typename BaseTask<BusMex>::TaskError;
    using SendMex = typename BaseTask<BusMex>::SendMex;

    explicit PotentialElectionTask() noexcept;

    PotentialElectionTask(const SendMex send_f) noexcept;
    PotentialElectionTask(
        const SendMex send_f,
        const ComputePotF pot_f,
        const LocalPotentialInfo& pot_info) noexcept;

    TaskError run(void) override;

  private:
    SendMex m_send_f=[](BusMex&){return BaseTask<BusMex>::BusStatus::Inactive;};
    ComputePotF m_compute_local_potential = [](){return 0;};
    LocalPotentialInfo& pot_info;
};
