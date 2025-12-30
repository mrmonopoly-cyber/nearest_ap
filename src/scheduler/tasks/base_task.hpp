#pragma once

#include <cstddef>

#include <nearest_ap/scheduler/tasks/task_list.hpp>

namespace nearest_ap {

  using Millis_t = std::size_t;

  class BaseTask_t
  {
    public:
      BaseTask_t() = delete;

      BaseTask_t(const TaskId id, const Millis_t freq) : m_id(id), m_freq(freq){}

      virtual void run(void) = 0;
      inline constexpr TaskId id() const noexcept
      {
        return m_id;
      }
      inline Millis_t freq() const noexcept
      {
        return m_freq;
      }

    protected:
      void update_freq(Millis_t f) noexcept
      {
        m_freq = f;
      }

    private:
      const TaskId m_id;
      Millis_t m_freq;
  };
}
