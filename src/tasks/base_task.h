#pragma once

#include <array>

namespace nearest_ap {

  enum class TaskError_t
  {
    Ok,
    Error,
  };

  class BaseTask_t
  {
    public:
      virtual TaskError_t run(void) = 0;
  };
}
