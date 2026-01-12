#include "spawner.h"
#include <cstdint>
#include <string_view>
#include <thread>

using namespace nearest_ap;

using namespace std::chrono;
using namespace std::this_thread;

void SpawnerLinux_t::_run_task(BaseTask_t*const t) noexcept
{
  constexpr auto s_runner_f = [](TaskWrapper* r_task)
  {
    while(r_task->can_execute())
    {
      r_task->run();
    }

    r_task->stopped();
  };

  const TaskId t_index = t->id();
  if (t_index < static_cast<TaskId>(InteractibleTask::TASK_COUNT))
  {
    m_tasks[t_index] = TaskWrapper{t};
    TaskWrapper& ref_task = m_tasks[t_index]; 
    std::thread th{s_runner_f, &ref_task};
    th.detach();
  }
}

void SpawnerLinux_t::start_task(BaseTask_t* t) noexcept 
{
  logger::UserLog<128>log{};
  log.append_msg("staring task: ");
  log.append_msg(t->id());
  log.append_msg(" with freq: ");
  log.append_msg(static_cast<uint32_t>(t->freq()));
  static_log(logger::Level::Debug, log);

  return _run_task(t);
}

void SpawnerLinux_t::stop_task(BaseTask_t* t) noexcept
{
  m_tasks[t->id()].stop();
}

SpawnerLinux_t::~SpawnerLinux_t()
{
  for(auto& task: m_tasks)
  {
    if (task.still_valid())
    {
      logger::UserLog<128>log{};
      log.append_msg("stopping task: ");
      log.append_msg(task.id());
      static_log(logger::Level::Debug, log);
      task.stop();
      while(!task.is_stopped()){
      }
    }
  }
}

bool SpawnerLinux_t::TaskWrapper::can_execute() const noexcept
{
  return m_run;
}

bool SpawnerLinux_t::TaskWrapper::still_valid() const noexcept
{
  return m_base_task != nullptr;
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

void SpawnerLinux_t::TaskWrapper::stopped() noexcept
{
  m_stopped = true;
}

bool SpawnerLinux_t::TaskWrapper::is_stopped() noexcept
{
  return m_stopped;
}

void SpawnerLinux_t::TaskWrapper::run() noexcept
{
  m_base_task->run();
  std::this_thread::sleep_for(std::chrono::milliseconds(m_base_task->freq()));
}
