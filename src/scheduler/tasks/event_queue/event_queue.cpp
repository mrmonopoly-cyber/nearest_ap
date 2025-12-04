#include "event_queue.hpp"

using namespace nearest_ap;

void EventWriter::push(const Event event) noexcept
{
  m_queue.push(event);
}

void EventWriter::push(const Event& event) noexcept
{
  m_queue.push(event);
}

void EventWriter::push(Event&& event) noexcept
{
  m_queue.push(std::move(event));
}


EventWriter::Event EventQueue::pop() noexcept
{
  return m_queue.front();
}

bool EventQueue::empty() const noexcept
{
  return m_queue.empty();
}
