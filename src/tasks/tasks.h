#pragma once

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

namespace nearest_ap {
  template<std::size_t payload_max_size = BaseTask<>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class Tasks
    {
      public:
        enum class SpawnTaskReturn
        {
          Ok,
          Error,
        };

        using BaseTask_t = BaseTask<payload_max_size>;
        using PotentialElectionTask_t = PotentialElectionTask<payload_max_size, default_num_nodes, tollerance>;
        using LeaderAliveTask_t = LeaderAliveTask<payload_max_size, default_num_nodes>;
        using BusReaderTask_t = BusReaderTask<payload_max_size, default_num_nodes,tollerance>;

        using SendMex = typename BaseTask_t::SendMex;
        using RecvMex = typename BaseTask_t::RecvMex;
        using DebugPrint = std::function<void(SpawnTaskReturn)>;
        using TaskSpawn = std::function<SpawnTaskReturn(BaseTask<payload_max_size>&)>;

        Tasks() = delete;

        void start() noexcept;

      private:
        const TaskSpawn m_task_spawn;
        const DebugPrint m_debug_print = [](SpawnTaskReturn){};
        const SendMex& m_send_f = [](BaseTask<>::BusMex&){return BaseTask_t::BusStatus::Inactive;};
        const RecvMex& m_recv_f = [](){while(true){}};

        PotentialElectionTask_t m_pot_task;
        LeaderAliveTask_t m_election_task;
        BusReaderTask_t m_bus_reader_task;
    };
};
