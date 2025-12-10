#pragma once

#include <cstddef>
#include <cstdint>

#include <nearest_ap/scheduler/tasks/task_list.hpp>

namespace nearest_ap {

  using Millis_t = std::size_t;

  class BaseTask_t
  {
    public:
      BaseTask_t() = delete;

      BaseTask_t(const TaskId id) : m_id(id){}

      virtual void run(void) = 0;
      inline constexpr TaskId id() const
      {
        return m_id;
      }

    private:
      const TaskId m_id;
  };
}
