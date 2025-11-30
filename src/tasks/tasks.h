#pragma once

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"

#include "base_task.h"

template<typename BusMex>
struct Tasks
{
      PotentialElectionTask<BusMex> m_pot_task;
      LeaderAliveTask<BusMex> m_election_task;
      BusReaderTask<BusMex> m_bus_reader_task;

      Tasks() = delete;
};
