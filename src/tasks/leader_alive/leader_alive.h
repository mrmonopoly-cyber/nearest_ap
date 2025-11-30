#include "../base_task.h"

template<typename BusMex>
class LeaderAliveTask : BaseTask<BusMex>
{
  public:
    using TaskError = typename BaseTask<BusMex>::TaskError;

    explicit LeaderAliveTask() noexcept;

    TaskError run(void) override;
};
