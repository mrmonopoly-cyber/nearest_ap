#pragma once

#include <queue>

#include <nearest_ap/scheduler/tasks/task_list.hpp>

namespace nearest_ap
{

  enum class TaskSignal
  {
    SUSPEND,
    RESUME,
  };

  class EventWriter
  {
    public:
      struct Event
      {
        const InteractibleTask m_task;
        const TaskSignal m_signal;
      };

      EventWriter() = default;
      EventWriter(const EventWriter&) = delete;
      EventWriter& operator=(const EventWriter&) = delete;
      EventWriter(EventWriter&&) = delete;
      EventWriter& operator=(EventWriter&&) = delete;

      void push(const Event) noexcept;
      void push(const Event&) noexcept;
      void push(Event&&) noexcept;

    protected:
      std::queue<Event> m_queue;
  };

  class EventQueue : public EventWriter
  {
    public:
      EventQueue() = default;

      EventQueue(const EventQueue&) = delete;
      EventQueue& operator=(const EventQueue&) = delete;

      EventQueue(EventQueue&&) = delete;
      EventQueue operator=(EventQueue&&) = delete;

      Event pop() noexcept;
      bool empty() const noexcept;
  };
}
