#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string_view>

#include "tasks/base_task.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {
  template<
    std::size_t mex_size = BaseTask<>::m_payload_max_size,
    uint32_t initial_num_nodes=VoteInfo<>::m_default_num_candidates,
    uint32_t tollerance=LocalPotentialInfo<>::m_tollerance>
  class Node
  {
    public:
      using Tasks = Tasks<initial_num_nodes, tollerance>;
      using Internal = Internal<initial_num_nodes, tollerance>;

      using SendMex = typename BaseTask<mex_size>::SendMex;
      using RecvMex = typename BaseTask<mex_size>::RecvMex;
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

      explicit Node(
          const Candidate::Id id,
          const SendMex send_f,
          const RecvMex recv_f,
          const TaskSpawn task_spawn_f) noexcept;

      explicit Node(
          const Candidate::Id id,
          const SendMex send_f,
          const RecvMex recv_f,
          const TaskSpawn task_spawn_f,
          const DebugPrint debug_print_f) noexcept;

      void async_start(void);
      void update_id(Candidate::Id id);

    private:
      Internal m_internal;
      Tasks m_tasks;
  };
}
