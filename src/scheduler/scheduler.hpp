#pragma once

#include "bus/bus.hpp"
#include "tasks/base_task.hpp"
#include "tasks/tasks.hpp"
#include <nearest_ap/internal/internal.hpp>

namespace nearest_ap
{
  template<typename SpawnerType >
    class Scheduler
    {
      public:
        using ComputePot_f = typename Internal_t::ComputePot_f;
        using BusMsg_t = typename Bus_t::Msg_t;
        using LeaderTaks_f = typename LeaderAliveTask_t::LeaderTask_f;


        Scheduler() = delete;
        Scheduler(const Scheduler&) =delete;
        Scheduler& operator=(const Scheduler&) =delete;

        Scheduler (Scheduler&&) =default;

        Scheduler(
            SpawnerType spawner,
            Bus_t& bus,
            LeaderTaks_f leader_task_f,
            Internal_t& internal) noexcept:
          m_spawner{std::move(spawner)},
          m_bus {bus},
          m_pot_election_task {bus, internal},
          m_alive_task {bus, internal, std::move(leader_task_f)},
          m_bus_reader_task {bus, internal}
        {
          _spawn_task();
        }

        Scheduler(
            SpawnerType spawner,
            Bus_t& bus,
            LeaderTaks_f leader_task_f,
            Internal_t& internal,
            const Millis_t bus_task_freq,
            const Millis_t pot_task_freq,
            const Millis_t alive_task_freq
            ) noexcept:
          m_spawner{std::move(spawner)},
          m_bus {bus},
          m_pot_election_task {bus, internal, pot_task_freq},
          m_alive_task {bus, internal, std::move(leader_task_f), alive_task_freq},
          m_bus_reader_task {bus, internal, bus_task_freq}
        {
          _spawn_task();
        }

      private:
        inline void _spawn_task(void) noexcept
        {
          m_spawner.start_task(&m_bus_reader_task);
          m_spawner.start_task(&m_pot_election_task);
          m_spawner.start_task(&m_alive_task);
        }

        SpawnerType m_spawner;
        Bus_t& m_bus;

        PotentialElectionTask_t m_pot_election_task;
        LeaderAliveTask_t m_alive_task;
        BusReaderTask_t m_bus_reader_task;
    };
};
