#pragma once

#include <cstdint>
#include <sys/socket.h>

#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
  class BusLinux_t : public Bus_t
  {
    public:
      BusLinux_t();

      BusLinux_t(const int max_connections);

      BusLinux_t(const BusLinux_t&) = delete;
      BusLinux_t& operator=(const BusLinux_t&) = delete;

      BusLinux_t(BusLinux_t&&) = default;
      BusLinux_t& operator=(BusLinux_t&&) = default;

      Msg_t Read() noexcept override;

      BusStatus_t Write(const Msg_t&) noexcept override;

      private:

      int m_socket;
  };
};

