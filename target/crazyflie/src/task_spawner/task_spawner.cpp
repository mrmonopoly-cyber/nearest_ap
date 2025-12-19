extern "C"
{
#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
}

#include <cstdint>
#include <task_spawner/task_spawner.hpp>

#include "task_spawner.hpp"

using namespace nearest_ap;

struct TaskWrapper
{
  TickType_t freq;
  BaseTask_t* task;
};

void task_runner(void* data)
{
  TaskWrapper task  = *reinterpret_cast<TaskWrapper*>(data);
  while(1)
  {
    task.task->run();
    vTaskDelay(task.freq);
  }
}

TaskCraziflieSpawner::TaskCraziflieSpawner()
  :m_tasks()
{
}

void TaskCraziflieSpawner::start_task(BaseTask_t* const task)
{
  return start_task(task, 0); //INFO: Maybe considering a minimum base frequency to do not surpass?
}

void TaskCraziflieSpawner::start_task(BaseTask_t* const task, Millis_t time)
{
  const uint16_t stack_size = 200;
  const TickType_t tick_delay =  time/ portTICK_PERIOD_MS;
  TaskWrapper full_task{tick_delay, task};
  TaskHandle_t* p_handler = m_tasks[task->id()];

  BaseType_t error = xTaskCreate(task_runner, "hh", stack_size, &full_task, tskIDLE_PRIORITY, p_handler);
  if (error != pdPASS)
  {
    //TODO: add error handling
  }
  vTaskDelay(10/ portTICK_PERIOD_MS);

}
