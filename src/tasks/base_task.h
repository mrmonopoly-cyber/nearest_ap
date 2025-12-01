#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <iterator>

template<std::size_t payload_max_size = 8>
class BaseTask
{
  public:
    static constexpr size_t m_payload_max_size = payload_max_size;
    using BusMex = std::array<std::byte, payload_max_size>;

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
