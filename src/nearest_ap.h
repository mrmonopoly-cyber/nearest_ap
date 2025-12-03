#pragma once

#include "tasks/base_task.h"
#include "tasks/tasks.h"
#include "internal/internal.h"

namespace nearest_ap {

  template<typename AddressType, std::size_t mex_size = Bus<AddressType>::m_payload_max_size>
  class Node
  {
    public:
      using Bus_t = Bus<AddressType, mex_size>;
      using Tasks_t = Tasks<AddressType, mex_size>;
      using LocalPotentialInfo_t = LocalPotentialInfo<AddressType>;
      using Internal_t = Internal<AddressType>;
      using VoteInfo_t = VoteInfo<AddressType>;

      using DebugPrint_t = typename Tasks_t::DebugPrint_t;
      using TaskSpawn_t = typename Tasks_t::TaskSpawn_t;
      using ComputePotF = typename Tasks_t::ComputePotF;

      Node() = delete;

      explicit Node(
          Bus_t& bus,
          const WaitFun_f& wait_f,
          const TaskSpawn_t& task_spawn_f,
          const ComputePotF& compute_pot_f) noexcept :
        m_internal
        {
          .m_current_user{},
          .m_vote_info{m_internal.m_current_user,m_internal. m_current_user},
          .m_local_potential_info
          {
            .m_local_node = m_internal.m_current_user,
            .m_leader = m_internal.m_current_user
          }
        },
        m_tasks
        {
          bus,
          wait_f,
          task_spawn_f,
          compute_pot_f,
          m_internal.m_local_potential_info,
          m_internal.m_vote_info,
        }
        {
        }

      explicit Node(
          Bus_t& bus,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      explicit Node(
          Bus_t& bus,
          const AddressType id,
          const TaskSpawn_t task_spawn_f) noexcept;

      explicit Node(
          Bus_t& bus,
          const AddressType id,
          const TaskSpawn_t task_spawn_f,
          const DebugPrint_t debug_print_f) noexcept;

      void async_start(void)
      {
        m_tasks.spawn_tasks();
      }
      void update_id(AddressType id)
      {
        m_internal.m_current_user = id;
      }

    private:
      Internal_t m_internal;
      Tasks_t m_tasks;
  };
}
