#pragma once

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

template<typename BusMex, uint32_t default_num_nodes>
struct Tasks
{
      PotentialElectionTask<BusMex, default_num_nodes> m_pot_task;
      LeaderAliveTask<BusMex, default_num_nodes> m_election_task;
      BusReaderTask<BusMex, default_num_nodes> m_bus_reader_task;

      Tasks() = delete;
};
