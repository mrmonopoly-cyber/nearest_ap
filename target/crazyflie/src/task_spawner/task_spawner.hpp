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
    struct TaskMetadata
    {
      TaskHandle_t* handler;
      nearest_ap::BaseTask_t* task;
      bool run;
    };


    using BaseTask_t = nearest_ap::BaseTask_t;
    using Millis_t = nearest_ap::Millis_t;

    TaskCraziflieSpawner();

    void start_task(BaseTask_t* const) override;
    void stop_task(BaseTask_t* const) override;

  private:
    std::array<TaskMetadata,3> m_tasks;
};
