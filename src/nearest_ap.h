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
    typename AddressType,
    std::size_t mex_size = BaseTask<AddressType>::m_payload_max_size,
    std::uint32_t initial_num_nodes=VoteInfo<>::m_default_num_candidates,
    std::uint32_t tollerance=LocalPotentialInfo<>::m_tollerance>
  class Node
  {
    public:
      using Tasks_t = Tasks<AddressType, mex_size, initial_num_nodes, tollerance>;

      using SendMex_t = typename Tasks_t::SendMex_t;
      using RecvMex_t = typename Tasks_t::RecvMex_t;
      using TaskSpawn_t = typename Tasks_t::TaskSpawn_t;
      using DebugPrint_t = typename Tasks_t::DebugPrint_t;

      Node() = delete;

      explicit Node(
          const SendMex_t send_f,
          const RecvMex_t recv_f,
          const TaskSpawn_t task_spawn_f) noexcept;

      explicit Node(
          const SendMex_t send_f,
          const RecvMex_t recv_f,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      explicit Node(
          const Candidate::Id id,
          const SendMex_t send_f,
          const RecvMex_t recv_f,
          const TaskSpawn_t task_spawn_f) noexcept;

      explicit Node(
          const Candidate::Id id,
          const SendMex_t send_f,
          const RecvMex_t recv_f,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      void async_start(void);
      void update_id(Candidate::Id id);

    private:
      Internal<initial_num_nodes, tollerance> m_internal;
      Tasks_t m_tasks;
  };
}
