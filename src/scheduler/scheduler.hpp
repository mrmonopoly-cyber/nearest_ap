#pragma once

#include "bus/bus.hpp"
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

        Scheduler(
            SpawnerType&& spawner,
            Bus_t& bus,
            LeaderTaks_f && leader_task_f,
            Internal_t& internal) noexcept:
          m_spawner{std::move(spawner)},
          m_bus {bus},
          m_event_queue(),
          m_pot_election_task {m_event_queue, bus, internal},
          m_alive_task {m_event_queue, bus, internal, leader_task_f},
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
        Bus_t& m_bus;
        EventQueue m_event_queue;

        PotentialElectionTask_t m_pot_election_task;
        LeaderAliveTask_t m_alive_task;
        BusReaderTask_t m_bus_reader_task;

        EventTask_t m_event_loop_task;
    };
};
