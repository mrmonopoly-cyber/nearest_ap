#pragma once

#include <cstddef>
#include <array>

namespace nearest_ap {
  enum class BusStatus_t
  {
    Ok,
    QueueFull,
    Inactive,
    UnknowError,
  };

  template<typename AddressType, std::size_t payload_max_size = 8>
  class Bus
  {
    public:
      struct Msg
      {
        AddressType id;
        std::array<std::byte, payload_max_size> payload; 
      };

      static constexpr size_t m_payload_max_size = payload_max_size;

      virtual Msg Read() const noexcept =0;
      virtual BusStatus_t Write(const Msg&) noexcept =0;
  };
}
