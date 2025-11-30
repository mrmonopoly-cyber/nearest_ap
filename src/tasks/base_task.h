#pragma once

#include <functional>

template<typename BusMex>
class BaseTask
{
  public:
    enum class BusStatus
    {
      Ok,
      QueueFull,
      Inactive,
      UnknowError,
    };

    enum class TaskError 
    {
      Ok,
      Error,
    };

    using RecvMex = std::function<BusMex()>;
    using SendMex = std::function<BusStatus(BusMex&)> ;

    virtual TaskError run(void) = 0;
};
