#pragma once

#include <array>
#include <functional>

namespace nearest_ap {
  enum class BusStatus_t
  {
    Ok,
    QueueFull,
    Inactive,
    UnknowError,
  };

  enum class TaskError_t
  {
    Ok,
    Error,
  };
  template<typename AddressType, std::size_t payload_max_size = 8>
    class BaseTask
    {
      public:
        struct BusMex{
          AddressType id;
          std::array<std::byte, payload_max_size> payload; 
        };
        using RecvMex_t = std::function<BusMex()>;
        using SendMex_t = std::function<BusStatus_t(BusMex&)> ;

        virtual TaskError_t run(void) = 0;

        static constexpr size_t m_payload_max_size = payload_max_size;
    };
}
