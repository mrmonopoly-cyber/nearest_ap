#pragma once

#include "tasks/tasks.hpp"
#include "../internal/internal.hpp"

namespace nearest_ap
{
  template<typename AddressType, typename BusType, typename SpawnerType >
    class Scheduler
    {
      public:
        using PotentialElectionTask_t = PotentialElectionTask<AddressType, BusType>;
        using LeaderAliveTask_t = LeaderAliveTask<AddressType, BusType>;
        using BusReaderTask_t = BusReaderTask<AddressType, BusType>;
        using Internal_t = Internal<AddressType>;
        using EventTask_t = EventTask<AddressType, BusType, SpawnerType>;

        using ComputePotF = typename PotentialElectionTask_t::ComputePotF;
        using BusMsg_t = typename BusType::Msg_t;


        Scheduler() = delete;

        Scheduler(
            SpawnerType&& spawner,
            BusType&& bus,
            ComputePotF&& compute_pot,
            Internal_t& internal) noexcept:
          m_spawner{std::move(spawner)},
          m_bus {std::move(bus)},
          m_event_queue(),
          m_pot_election_task {m_event_queue, bus, std::move(compute_pot), internal},
          m_alive_task {m_event_queue, bus, internal},
          m_bus_reader_task {m_event_queue, bus, internal},
          m_event_loop_task(m_event_queue, m_spawner, m_pot_election_task, m_alive_task, m_bus_reader_task)
        {
        }

        void spawn_tasks() noexcept
        {
          m_spawner.attach_timer_to_task(m_event_loop_task, Millis_t{10});
          m_spawner.attach_timer_to_task(m_pot_election_task, Millis_t{200});
          m_spawner.start_task(&m_bus_reader_task);
        }

      private:
        SpawnerType m_spawner;
        BusType m_bus;
        EventQueue m_event_queue;

        PotentialElectionTask_t m_pot_election_task;
        LeaderAliveTask_t m_alive_task;
        BusReaderTask_t m_bus_reader_task;

        EventTask_t m_event_loop_task;
    };
};
