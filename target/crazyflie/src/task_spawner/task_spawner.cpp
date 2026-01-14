extern "C"
{
#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "debug.h"
}

#include <cstdint>

#include <task_spawner/task_spawner.hpp>

using namespace nearest_ap;

static void _task_runner(void* data)
{
  TaskCraziflieSpawner::TaskMetadata task  = *reinterpret_cast<TaskCraziflieSpawner::TaskMetadata*>(data);
  while(task.run)
  {
    task.task->run();
    vTaskDelay(task.task->freq());
  }
}

TaskCraziflieSpawner::TaskCraziflieSpawner()
  :m_tasks()
{
}

void TaskCraziflieSpawner::start_task(BaseTask_t* const task)
{
  if (task->id() >= m_tasks.size())
  {
    DEBUG_PRINT("%s:%d, %s. Invalid task id: %d", __FILE__, __LINE__, __func__, task->id());
    return;
  }

  BaseType_t error ={};
  TaskMetadata& metadata = m_tasks[task->id()];
  metadata.task = task;
  metadata.run = true;
  const uint16_t stack_size = 200;

  error = xTaskCreate(_task_runner, "hh", stack_size, &metadata, tskIDLE_PRIORITY, metadata.handler);
  while(error != pdPASS)
  {
    DEBUG_PRINT("failed starting task: %d\n", task->id());
    vTaskDelay(50/ portTICK_PERIOD_MS);
    error = xTaskCreate(_task_runner, "hh", stack_size, &metadata, tskIDLE_PRIORITY, metadata.handler);
  }

  vTaskDelay(10/ portTICK_PERIOD_MS);
}

void TaskCraziflieSpawner::stop_task(BaseTask_t* const task)
{
  if (task->id() >= m_tasks.size())
  {
    DEBUG_PRINT("%s:%d, %s. Invalid task id: %d", __FILE__, __LINE__, __func__, task->id());
    return;
  }

  m_tasks[task->id()].run = false;
}
