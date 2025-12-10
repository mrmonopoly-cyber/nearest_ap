#pragma once

#include <array>
#include <queue>
#include <sys/socket.h>

#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
  class BusLinux_t : public Bus_t
  {
    public:
      using socket_t = int;

      explicit BusLinux_t() noexcept;

      BusLinux_t(const int max_connections) noexcept;

      BusLinux_t(const BusLinux_t&) = delete;
      BusLinux_t& operator=(const BusLinux_t&) = delete;

      BusLinux_t(BusLinux_t&&) = default;
      BusLinux_t& operator=(BusLinux_t&&) = default;

      void Accept_connections() noexcept;

      std::optional<Msg_t> Read() noexcept override;

      BusStatus_t Write(const Msg_t&) noexcept override;

    private:
      static constexpr std::size_t m_max_clients = 20;
      socket_t m_socket;
      std::size_t m_client_connected;
      std::array<socket_t, m_max_clients> m_clients;
      std::queue<Msg_t> m_msg_queue;
  };
};

