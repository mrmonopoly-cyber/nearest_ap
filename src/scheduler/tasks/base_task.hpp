#pragma once

#include <cstddef>

namespace nearest_ap {

  using Millis_t = std::size_t;

  class BaseTask_t
  {
    public:
      virtual void run(void) = 0;
  };
}
