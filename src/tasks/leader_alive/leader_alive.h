#include "../base_task.h"
#include "../../internal/internal.h"

template<typename BusMex>
class LeaderAliveTask : BaseTask<BusMex>
{
  public:
    using TaskError = typename BaseTask<BusMex>::TaskError;
    using SendMex = typename BaseTask<BusMex>::SendMex;

    explicit LeaderAliveTask() noexcept;

    LeaderAliveTask(const SendMex send_f, const VoteInfo& vote_info) noexcept;

    TaskError run(void) override;
  private:
    SendMex m_send_f;
    const VoteInfo& m_vote_info;
};
