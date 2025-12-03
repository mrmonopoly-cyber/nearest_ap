#pragma once

#include <cstddef>
#include <array>

#include <pb.h>

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
      struct Msg_t
      {
        AddressType m_id;
        std::array<pb_byte_t, payload_max_size> m_payload; 
      };

      static constexpr size_t m_payload_max_size = payload_max_size;

      virtual Msg_t Read() const noexcept =0;
      virtual BusStatus_t Write(const Msg_t&) noexcept =0;
  };
}
