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
          BusReaderTask_t& bus_reader_task) noexcept;

      void run(void) noexcept override;

    private:
      EventQueue& m_event_queue;
      Spawner_t& m_spawner;

      PotentialElectionTask_t& m_pot_election_task;
      LeaderAliveTask_t& m_alive_task;
      BusReaderTask_t& m_bus_reader_task;

  };
};
