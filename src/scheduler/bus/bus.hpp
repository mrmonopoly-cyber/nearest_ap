#pragma once

#include <cstddef>
#include <array>

#include <cstdint>
#include <pb.h>

namespace nearest_ap {
  enum class BusStatus_t
  {
    Ok,
    QueueFull,
    Inactive,
    UnknowError,
  };

  class Bus_t
  {
    public:
      using AddressType_t = std::uint32_t;
      static constexpr std::size_t m_payload_max_size = 16;

      struct Msg_t
      {
        std::array<pb_byte_t, m_payload_max_size> m_payload; 
      };

      virtual Msg_t Read() noexcept =0;
      virtual BusStatus_t Write(const Msg_t&) noexcept =0;

      virtual ~Bus_t() = default;
  };
}
