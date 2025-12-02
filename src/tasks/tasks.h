#pragma once

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

namespace nearest_ap {
  enum class SpawnTaskReturn
  {
    Ok,
    Error,
  };

  template<
    typename AddressType,
    std::size_t payload_max_size = BaseTask<AddressType>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class Tasks
    {
      public:

        using BaseTask_t = BaseTask<AddressType, payload_max_size>;
        using PotentialElectionTask_t = PotentialElectionTask<AddressType, payload_max_size, default_num_nodes, tollerance>;
        using LeaderAliveTask_t = LeaderAliveTask<AddressType, payload_max_size, default_num_nodes>;
        using BusReaderTask_t = BusReaderTask<AddressType, payload_max_size, default_num_nodes,tollerance>;

        using BusMex_t = typename BaseTask_t::BusMex;
        using SendMex_t = typename BaseTask_t::SendMex_t;
        using RecvMex_t = typename BaseTask_t::RecvMex_t;
        using DebugPrint_t = std::function<void(SpawnTaskReturn)>;
        using TaskSpawn_t = std::function<SpawnTaskReturn(BaseTask_t&)>;

        Tasks() = delete;

        void start() noexcept;

      private:
        const TaskSpawn_t m_task_spawn;
        const DebugPrint_t m_debug_print = [](SpawnTaskReturn){};
        const SendMex_t& m_send_f = [](BusMex_t&){return BaseTask_t::BusStatus::Inactive;};
        const RecvMex_t& m_recv_f = [](){while(true){}};

        PotentialElectionTask_t m_pot_task;
        LeaderAliveTask_t m_election_task;
        BusReaderTask_t m_bus_reader_task;
    };
};
