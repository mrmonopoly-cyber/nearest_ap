#pragma once

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

namespace nearest_ap {
  template<typename BusMex, uint32_t default_num_nodes>
    class Tasks
    {
      public:
        enum class SpawnTaskReturn
        {
          Ok,
          Error,
        };

        using DebugPrint = std::function<void(SpawnTaskReturn)>;
        using TaskSpawn = std::function<SpawnTaskReturn(BaseTask<BusMex>&)>;

        Tasks() = delete;

        void start();

      private:
        TaskSpawn m_task_spawn;
        DebugPrint m_debug_print = [](SpawnTaskReturn){};

        PotentialElectionTask<BusMex, default_num_nodes> m_pot_task;
        LeaderAliveTask<BusMex, default_num_nodes> m_election_task;
        BusReaderTask<BusMex, default_num_nodes> m_bus_reader_task;

    };
};
