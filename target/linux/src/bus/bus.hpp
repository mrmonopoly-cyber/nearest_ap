#pragma once

#include <atomic>
#include <array>
#include <mutex>
#include <queue>
#include <sys/socket.h>

#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
  class BusLinux_t : public Bus_t
  {
    public:
      using socket_t = int;
      using Id = uint;

      BusLinux_t() noexcept;

      BusLinux_t(const BusLinux_t&) = delete;
      BusLinux_t& operator=(const BusLinux_t&) = delete;

      BusLinux_t(BusLinux_t&&) = delete;
      BusLinux_t& operator=(BusLinux_t&&) = delete;


      void enstablis_connection(void) noexcept;

      std::optional<Msg_t> Read(void) noexcept override;
      BusStatus_t Write(const Msg_t&) noexcept override;

    public:
      static constexpr std::size_t m_max_clients = 25;

    private:
      static void _Accept(BusLinux_t* const self) noexcept;
      void _socket_setup(void) noexcept;

    private:
      const Id m_id;
      socket_t m_socket;
      std::array<std::optional<socket_t>, m_max_clients> m_clients;
      std::queue<Msg_t> m_msg_queue;
      std::mutex m_msg_queue_lock;
      std::atomic_uint8_t m_client_connected;
  };
};

