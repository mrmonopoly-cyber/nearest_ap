#pragma once

#include <nearest_ap/scheduler/tasks/base_task.hpp>
#include <nearest_ap/scheduler/tasks/user_tasks/user_tasks.hpp>
#include <nearest_ap/scheduler/tasks/spawner/spawner.hpp>

namespace nearest_ap
{
  class EventTask_t : public BaseTask_t
  {
    public:
      EventTask_t() = delete;
      EventTask_t(const EventTask_t&) =delete;
      EventTask_t& operator=(const EventTask_t&) =delete;

      EventTask_t(EventTask_t&&) =delete;
      EventTask_t operator=(EventTask_t&&) =delete;

      EventTask_t(
          EventQueue& event_queue,
          Spawner_t& spawner,
          PotentialElectionTask_t& potential_election_task,
          LeaderAliveTask_t& leader_alive_task,
          BusReaderTask_t& bus_reader_task) :
        BaseTask_t(),
        m_event_queue(event_queue),
        m_spawner(spawner),
        m_pot_election_task(potential_election_task),
        m_alive_task(leader_alive_task),
        m_bus_reader_task(bus_reader_task)
    {
    }

      void run(void) override
      {
        if (!m_event_queue.empty())
        {
          EventQueue::Event e = m_event_queue.pop();
          BaseTask_t* task = nullptr;
          switch (e.m_task)
          {
            case InteractibleTask::BUS_READER:
              task = &m_bus_reader_task;
              break;
            case InteractibleTask::LEADER_ALIVE:
              task = &m_alive_task;
              break;
            case InteractibleTask::POTENTIAL_ELECTION:
              task = &m_pot_election_task;
              break;
            default:
              return;
          }

          switch (e.m_signal)
          {
            case TaskSignal::SUSPEND:
              m_spawner.suspend_task(task);
              break;
            case TaskSignal::RESUME:
              m_spawner.resume_task(task);
              break;
            default:
              return;
          }
        }
      }

    private:
      EventQueue& m_event_queue;
      Spawner_t& m_spawner;

      PotentialElectionTask_t& m_pot_election_task;
      LeaderAliveTask_t& m_alive_task;
      BusReaderTask_t& m_bus_reader_task;

  };
};
