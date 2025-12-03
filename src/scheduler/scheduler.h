#pragma once

#include "base_task.h"
#include "spawner.h"

#include "leader_alive/leader_alive.h"
#include "bus_reader/bus_reader.h"
#include "potential_election/potential_election.h"


namespace nearest_ap
{
  template<typename AddressType, typename BusType, typename SpawnerType >
  class Scheduler
  {
    public:
      using Vector_t = std::vector<AddressType>;

      using Internal_t = Internal<AddressType>;

      using PotentialElectionTask_t = PotentialElectionTask<AddressType, BusType>;
      using LeaderAliveTask_t = LeaderAliveTask<AddressType, BusType>;
      using BusReaderTask_t = BusReaderTask<AddressType, BusType>;

      using ComputePotF = typename PotentialElectionTask_t::ComputePotF;
      using BusMsg_t = typename BusType::Msg_t;

      Scheduler() = delete;

      Scheduler(
          SpawnerType&& spawner,
          BusType&& bus,
          const ComputePotF&& compute_pot,
          Internal_t& internal) noexcept:
        m_spawner{std::move(spawner)},
        m_bus {std::move(bus)},
        m_pot_election_task {bus, std::move(compute_pot), internal},
        m_alive_task {bus, internal},
        m_bus_reader_task {bus, internal}
        {
        }

      void spawn_tasks() noexcept
      {
      }

    private:
      SpawnerType m_spawner;
      BusType m_bus;

      PotentialElectionTask_t m_pot_election_task;
      LeaderAliveTask_t m_alive_task;
      BusReaderTask_t m_bus_reader_task;
  };
};
