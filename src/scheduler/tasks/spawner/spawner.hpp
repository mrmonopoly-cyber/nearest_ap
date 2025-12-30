#pragma once

#include <nearest_ap/scheduler/tasks/base_task.hpp>

namespace nearest_ap 
{
  class Spawner_t
  {
    public:
      virtual void start_task(BaseTask_t* const) =0;

      virtual ~Spawner_t() = default;
  };
};
