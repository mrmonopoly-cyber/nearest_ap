#pragma once

#include <cstddef>

#include "../base_task.hpp"
#include "../event_queue/event_queue.hpp"

namespace nearest_ap {

  using Millis_t = std::size_t;

  class UserTask_t : public BaseTask_t 
  {
    public:
      UserTask_t() = delete;

      UserTask_t(EventWriter& queue) noexcept;
    private:
      EventWriter& m_queue;
  };
}
