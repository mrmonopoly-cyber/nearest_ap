#pragma once

#include <cstdint>

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

namespace nearest_ap {
  template<std::size_t payload_max_size = BaseTask<>::m_payload_max_size,
    uint32_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    uint32_t tollerance = LocalPotentialInfo<>::m_tollerance>
    class Tasks
    {
      public:
        enum class SpawnTaskReturn
        {
          Ok,
          Error,
        };

        Tasks() = delete;

        void start();

      private:
        using DebugPrint = std::function<void(SpawnTaskReturn)>;
        using TaskSpawn = std::function<SpawnTaskReturn(BaseTask<payload_max_size>&)>;

        using PotentialElectionTask = PotentialElectionTask<payload_max_size, default_num_nodes, tollerance>;
        using LeaderAliveTask =LeaderAliveTask<payload_max_size, default_num_nodes>;
        using BusReaderTask = BusReaderTask<payload_max_size, default_num_nodes,tollerance>;


        const TaskSpawn m_task_spawn;
        const DebugPrint m_debug_print = [](SpawnTaskReturn){};

        PotentialElectionTask m_pot_task;
        LeaderAliveTask m_election_task;
        BusReaderTask m_bus_reader_task;
    };
};
