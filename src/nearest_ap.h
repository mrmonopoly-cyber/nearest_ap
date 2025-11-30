#pragma once

#include <cstdint>
#include <functional>

#include "internal/potential_info/potential_info.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t initial_num_nodes=4>
  class Node
  {
    public:
      using SendMex = typename BaseTask<BusMex>::SendMex;
      using RecvMex = typename BaseTask<BusMex>::RecvMex;
      using TaskSpawn = typename Tasks<BusMex, initial_num_nodes>::TaskSpawn;
      using DebugPrint = typename Tasks<BusMex, initial_num_nodes>::DebugPrint;

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
      Internal<initial_num_nodes> m_internal;
      Tasks<BusMex, initial_num_nodes> m_tasks;
  };
}
