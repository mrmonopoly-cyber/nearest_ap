#pragma once

#include <cstddef>

#include <nearest_ap/scheduler/tasks/task_list.hpp>

namespace nearest_ap {

  using Millis_t = std::size_t;

  class BaseTask_t
  {
    public:
      BaseTask_t() = delete;

      BaseTask_t(const TaskId id, const Millis_t freq) noexcept :
        m_id(id), m_base_freq(freq), m_freq(freq)
        {
        }

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
      void update_freq_by_factor(float f) noexcept
      {
        const auto inc = static_cast<float>(m_base_freq) * f;
        m_freq += inc;
      }

      void reset_freq() noexcept
      {
        m_freq = m_base_freq;
      }

    private:
      const TaskId m_id;
      const Millis_t m_base_freq;
      Millis_t m_freq;
  };
}
