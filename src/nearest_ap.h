#pragma once

#include <functional>

#include "internal/potential_info/potential_info.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {


  enum class SpawnTaskReturn
  {
    Ok,
    Error,
  };


  template<typename BusMex, int num_nodes>
  class Node
  {
    public:
      using DebugPrint = std::function<void(SpawnTaskReturn)>;
      using TaskSpawn = std::function<SpawnTaskReturn(BaseTask<BusMex>&)>;
      using SendMex = typename BaseTask<BusMex>::SendMex;
      using RecvMex = typename BaseTask<BusMex>::RecvMex;

      Node() = delete;

      explicit Node(
          const SendMex send_f,
          const RecvMex recv_f,
          const TaskSpawn task_spawn_f) noexcept;

      explicit Node(
          const SendMex send_f,
          const RecvMex recv_f,
          const TaskSpawn task_spawn_f,
          const DebugPrint debug_print_f) noexcept;

      void async_start(void);

    private:
      TaskSpawn m_task_spawn;
      DebugPrint m_debug_print = [](SpawnTaskReturn){};

      Internal m_internal;
      Tasks<BusMex> m_tasks;
  };
}
