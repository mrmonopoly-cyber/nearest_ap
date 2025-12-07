#pragma once

#include <nearest_ap/scheduler/tasks/base_task.hpp>

namespace nearest_ap 
{
  class Spawner_t
  {
    public:
      virtual void attach_timer_to_task(const BaseTask_t&, Millis_t) =0;
      virtual void start_task(BaseTask_t*) =0;
      virtual void suspend_task(BaseTask_t*) =0;
      virtual void resume_task(BaseTask_t*) =0;

      virtual ~Spawner_t() = default;
  };
};
