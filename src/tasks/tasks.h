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
    Task() = delete;

    virtual TaskError run(void) = 0;
};
