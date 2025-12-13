#include "spawner.h"
#include <thread>
#include <iostream>

using namespace nearest_ap;

using namespace std::chrono;
using namespace std::this_thread;

void SpawnerLinux_t::_run_task(BaseTask_t*const t, const Millis_t freq) noexcept
{
  const TaskId t_index = t->id();
  if (t_index < static_cast<TaskId>(InteractibleTask::TASK_COUNT))
  {
    m_tasks[t_index] = TaskWrapper{t, freq};
    TaskWrapper& ref_task = m_tasks[t_index]; 
    std::thread th{s_runner_f, &ref_task};
    th.detach();
  }
}

void SpawnerLinux_t::start_task(BaseTask_t* t) noexcept 
{
  return start_task(t,0);
}

void SpawnerLinux_t::start_task(BaseTask_t* t, Millis_t f) noexcept
{
  std::cout
    << "staring task: " << t->id()
    << " with freq: " << f
    << std::endl;

  _run_task(t,f);
}

SpawnerLinux_t::~SpawnerLinux_t()
{
  for(auto& task: m_tasks)
  {
    std::cout
      << "stopping task: "
      << task.id()
      << std::endl;
    task.stop();
  }
}

SpawnerLinux_t::TaskWrapper& SpawnerLinux_t::TaskWrapper::operator=(TaskWrapper&&other)
{
  if (this != &other)
  {
    this->m_base_task = other.m_base_task;
    this->m_run = other.m_run;
    this->m_freq = other.m_freq;

    other.m_base_task = nullptr;
    other.m_run = false;
    other.m_freq =0;
  }
  return *this;
}

bool SpawnerLinux_t::TaskWrapper::can_execute() const noexcept
{
  return m_run;
}

TaskId SpawnerLinux_t::TaskWrapper::id() const noexcept
{
  if (m_base_task)
  {
    return m_base_task->id();
  }
  return 99;
}

void SpawnerLinux_t::TaskWrapper::stop() noexcept
{
  m_run = false;
}

void SpawnerLinux_t::TaskWrapper::run() noexcept
{
  m_base_task->run();
  std::this_thread::sleep_for(std::chrono::milliseconds(m_freq));
}
