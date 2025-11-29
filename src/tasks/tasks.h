#pragma once

#include <functional>

class Task
{
  public:
    enum class TaskError 
    {
      Ok,
      Error,
    };

    using TFun = std::function<TaskError(void*)>;

    Task() = delete;
    explicit Task(void* const, const TFun) noexcept;
    explicit Task(void* const, const TFun&&) noexcept;

    TaskError run(void);

  private:
    void* const m_data;
    const TFun m_fun;
};
