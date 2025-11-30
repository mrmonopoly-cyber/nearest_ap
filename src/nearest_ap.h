#pragma once

#include <cstdint>
#include <functional>

#include "internal/potential_info/potential_info.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t initial_num_nodes=4, uint32_t tollerance=0>
  class Node
  {
    public:
      using Tasks = Tasks<BusMex, initial_num_nodes, tollerance>;
      using Internal = Internal<initial_num_nodes, tollerance>;

      using SendMex = typename BaseTask<BusMex>::SendMex;
      using RecvMex = typename BaseTask<BusMex>::RecvMex;
      using TaskSpawn = typename Tasks::TaskSpawn;
      using DebugPrint = typename Tasks::DebugPrint;

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
      Internal m_internal;
      Tasks m_tasks;
  };
}
