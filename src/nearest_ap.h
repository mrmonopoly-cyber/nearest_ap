#pragma once

#include "tasks/bus/bus.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {
  template<
    typename AddressType,
    std::size_t mex_size = Bus<AddressType>::m_payload_max_size,
    std::size_t initial_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance>
  class Node
  {
    public:
      using Tasks_t = Tasks<AddressType, mex_size, initial_num_nodes, tollerance>;
      using Internal_t = Internal<initial_num_nodes, tollerance>;
      using Bus_t = Bus<AddressType, mex_size>;

      using BusMex_t = typename Bus_t::Msg;
      using TaskSpawn_t = typename Tasks_t::TaskSpawn_t;
      using DebugPrint_t = typename Tasks_t::DebugPrint_t;


      Node() = delete;

      explicit Node(
          Bus_t& bus,
          const TaskSpawn_t task_spawn_f) noexcept;

      explicit Node(
          Bus_t& bus,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      explicit Node(
          Bus_t& bus,
          const Candidate_t::Id id,
          const TaskSpawn_t task_spawn_f) noexcept;

      explicit Node(
          Bus_t& bus,
          const Candidate_t::Id id,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      void async_start(void);
      void update_id(Candidate_t::Id id);

    private:
      Internal_t m_internal;
      Tasks_t m_tasks;
  };
}
