#pragma once

extern "C"
{
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
}

#include <array>

#include <nearest_ap/scheduler/tasks/spawner/spawner.hpp>

class TaskCraziflieSpawner : public nearest_ap::Spawner_t
{
  public:
    using BaseTask_t = nearest_ap::BaseTask_t;
    using Millis_t = nearest_ap::Millis_t;

    TaskCraziflieSpawner();

    void attach_timer_to_task(BaseTask_t&, Millis_t);
    void start_task(BaseTask_t*);
    void suspend_task(BaseTask_t*);
    void resume_task(BaseTask_t*);

  private:
    std::array<TaskHandle_t*,3> m_tasks;
};
