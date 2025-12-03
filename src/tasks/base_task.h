#pragma once

#include <array>
#include <functional>
#include <ctime>

namespace nearest_ap {

  using Millis_t = std::size_t;
  using WaitFun_f = std::function<void(Millis_t)>;

  class BaseTask_t
  {
    public:
      virtual void run(void) = 0;
  };
}
