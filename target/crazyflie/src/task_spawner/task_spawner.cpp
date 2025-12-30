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

void task_runner(void* data)
{
  BaseTask_t* task  = reinterpret_cast<BaseTask_t*>(data);
  while(1)
  {
    task->run();
    vTaskDelay(task->freq());
  }
}

TaskCraziflieSpawner::TaskCraziflieSpawner()
  :m_tasks()
{
}

void TaskCraziflieSpawner::start_task(BaseTask_t* const task)
{
  const uint16_t stack_size = 200;
  TaskHandle_t* p_handler = m_tasks[task->id()];

  BaseType_t error = xTaskCreate(task_runner, "hh", stack_size, task, tskIDLE_PRIORITY, p_handler);
  if (error != pdPASS)
  {
    //TODO: add error handling
  }
  vTaskDelay(10/ portTICK_PERIOD_MS);

}
