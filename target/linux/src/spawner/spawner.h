#pragma once

#include <array>
#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
class SpawnerLinux_t : public Spawner_t
  {
    private:
      class TaskWrapper 
      {
        public:
          TaskWrapper(
              const bool m_run=false,
              BaseTask_t *const m_base_task=nullptr) noexcept
              : m_run(m_run), m_base_task(m_base_task) {}

          TaskWrapper(BaseTask_t *const m_base_task) noexcept
              : m_run(true), m_base_task(m_base_task) {}

          TaskWrapper(TaskWrapper&&) =default;
          TaskWrapper& operator=(TaskWrapper&&) = default;

          bool can_execute() const noexcept;
          bool still_valid() const noexcept;
          TaskId id() const noexcept;

          void stop() noexcept;
          void run() noexcept;

        private:
          bool m_run;
          BaseTask_t* m_base_task;
      };

    public:
      explicit SpawnerLinux_t()
      {
        static_log(logger::Level::Debug, "creating new empty task");
      }

      SpawnerLinux_t(const SpawnerLinux_t& ) =delete;
      SpawnerLinux_t operator=(const SpawnerLinux_t& ) =delete;

      SpawnerLinux_t(SpawnerLinux_t&& ) = default;
      SpawnerLinux_t& operator=(SpawnerLinux_t&& ) = delete;

      void start_task(BaseTask_t* t) noexcept override;

      ~SpawnerLinux_t();

    private:
      void _run_task(BaseTask_t*const t) noexcept;

    private:
      static constexpr TaskId s_num_tasks = static_cast<TaskId>(InteractibleTask::TASK_COUNT);
      static constexpr auto s_runner_f = 
        [](TaskWrapper* r_task){while(r_task->can_execute())r_task->run();};

      std::array<TaskWrapper, s_num_tasks> m_tasks;
  };
}
