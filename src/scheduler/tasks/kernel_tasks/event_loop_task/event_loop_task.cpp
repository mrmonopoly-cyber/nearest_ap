


#include "event_loop_task.hpp"

using namespace nearest_ap;

EventTask_t::EventTask_t(
    EventQueue& event_queue,
    Spawner_t& spawner,
    PotentialElectionTask_t& potential_election_task,
    LeaderAliveTask_t& leader_alive_task,
    BusReaderTask_t& bus_reader_task) noexcept:
  BaseTask_t(),
  m_event_queue(event_queue),
  m_spawner(spawner),
  m_pot_election_task(potential_election_task),
  m_alive_task(leader_alive_task),
  m_bus_reader_task(bus_reader_task)
{}

void EventTask_t::run(void) noexcept
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
