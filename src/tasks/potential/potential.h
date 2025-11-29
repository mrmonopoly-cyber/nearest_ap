#pragma once

#include <functional>

#include "../tasks.h"
#include "../../internal/potential_info/potential_info.h"

class PotentialTask : public Task
{
  public:
    using ComputePotF = std::function<int()>;

    PotentialTask() =delete;
    PotentialTask(const ComputePotF pot_f, const LocalPotentialInfo& pot_info);
    TaskError run(void) override;

  private:
    ComputePotF m_compute_local_potential;
    LocalPotentialInfo& pot_info;
};
