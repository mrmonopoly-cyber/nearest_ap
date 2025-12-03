#pragma once

#include "base_task.h"
#include "bus/bus.h"

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

namespace nearest_ap
{
  enum class SpawnTaskReturn
  {
    Ok,
    Error,
  };
  
  template<typename AddressType, std::size_t mex_size = Bus<AddressType>::m_payload_max_size>
  class Tasks
  {
    public:
      using Bus_t = Bus<AddressType, mex_size>;
      using Vector_t = std::vector<AddressType>;
      using PotentialElectionTask_t = PotentialElectionTask<AddressType, mex_size>;
      using LeaderAliveTask_t = LeaderAliveTask<AddressType, mex_size>;
      using BusReaderTask_t = BusReaderTask<AddressType, mex_size>;
      using LocalPotentialInfo_t = LocalPotentialInfo<AddressType>;
      using VoteInfo_t = VoteInfo<AddressType>;

      using ComputePotF = typename PotentialElectionTask_t::ComputePotF;
      using BusMsg_t = typename Bus_t::Msg;
      using DebugPrint_t = std::function<void(SpawnTaskReturn)>;
      using TaskSpawn_t = std::function<SpawnTaskReturn(BaseTask_t&)>;

      Tasks() = delete;

      Tasks(
          Bus_t& bus,
          const WaitFun_f& wait_fun,
          const TaskSpawn_t& task_spawn,
          const ComputePotF& compute_pot,
          LocalPotentialInfo_t& pot_info,
          VoteInfo_t& vote_info) noexcept:
        m_wait_fun(wait_fun),
        m_task_spawn(task_spawn),
        m_pot_election_task{bus, compute_pot, pot_info, vote_info},
        m_alive_task{m_wait_fun, bus, pot_info, vote_info},
        m_bus_reader_task{bus, vote_info, pot_info}
        {
        }

      SpawnTaskReturn spawn_tasks() noexcept
      {
        if(
            (m_task_spawn(m_bus_reader_task)==SpawnTaskReturn::Error) ||
            (m_task_spawn(m_alive_task)==SpawnTaskReturn::Error) ||
            (m_task_spawn(m_pot_election_task)==SpawnTaskReturn::Error))
        {
          return SpawnTaskReturn::Error;
        }
      }

    private:
      const WaitFun_f m_wait_fun;
      const TaskSpawn_t m_task_spawn;
      const DebugPrint_t m_debug_print = [](SpawnTaskReturn){};
      PotentialElectionTask_t m_pot_election_task;
      LeaderAliveTask_t m_alive_task;
      BusReaderTask_t m_bus_reader_task;
  };
};
