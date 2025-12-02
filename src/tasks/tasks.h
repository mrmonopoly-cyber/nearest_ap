#pragma once

#include "bus/bus.h"

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

namespace nearest_ap {
  enum class SpawnTaskReturn
  {
    Ok,
    Error,
  };

  template<
    typename AddressType,
    std::size_t payload_max_size = Bus<AddressType>::m_payload_max_size,
    std::size_t default_num_nodes = VoteInfo<>::m_default_num_candidates,
    std::size_t tollerance = LocalPotentialInfo<>::m_tollerance >
    class Tasks
    {
      public:

        using PotentialElectionTask_t = PotentialElectionTask<AddressType, payload_max_size, default_num_nodes, tollerance>;
        using LeaderAliveTask_t = LeaderAliveTask<AddressType, payload_max_size, default_num_nodes>;
        using BusReaderTask_t = BusReaderTask<AddressType, payload_max_size, default_num_nodes,tollerance>;
        using Bus_t = Bus<AddressType, payload_max_size>;

        using DebugPrint_t = std::function<void(SpawnTaskReturn)>;
        using TaskSpawn_t = std::function<SpawnTaskReturn(BaseTask_t&)>;

        Tasks() = delete;

        void start() noexcept;

      private:
        const TaskSpawn_t m_task_spawn;
        const DebugPrint_t m_debug_print = [](SpawnTaskReturn){};

        Bus_t& m_bus;
        PotentialElectionTask_t m_pot_task;
        LeaderAliveTask_t m_election_task;
        BusReaderTask_t m_bus_reader_task;
    };
};
