#pragma once

#include "bus/fake_radio_bus/fake_radio_bus.hpp"
#include <atomic>
#include <cstdint>
#include <zmq.hpp>
#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
  class BusLinux_t : public Bus_t
  {
    public:
      BusLinux_t(const uint32_t prob=0) noexcept;

      BusLinux_t(const BusLinux_t&) = delete;
      BusLinux_t& operator=(const BusLinux_t&) = delete;

      BusLinux_t(BusLinux_t&&) = delete;
      BusLinux_t& operator=(BusLinux_t&&) = delete;

      inline void enstablis_connection(int num_clients) noexcept
      {
        m_radio_bus.enstablis_connection(num_clients);
      }

      inline void enable(void) noexcept
      {
        m_enable = true;
      }

      inline void disable(void) noexcept
      {
        m_enable = false;
      }

      std::optional<Msg_t> Read(void) noexcept override;
      BusStatus_t Write(const Msg_t&) noexcept override;

      ~BusLinux_t() noexcept;

    private:
      void p2pcallbackHandler(P2PPacket* packet) noexcept;

      static void _p2pcallbackHandler(void* obj, P2PPacket* packet) noexcept
      {
        BusLinux_t* self = reinterpret_cast<BusLinux_t*>(obj);
        self->p2pcallbackHandler(packet);
      }

      uint8_t _next(const uint8_t v) const noexcept;

    private:
      RadioBus m_radio_bus;
      static const constexpr uint32_t s_queue_size = 128;
      std::array<P2PPacket, s_queue_size> m_msg_queue{};
      std::atomic_uint8_t m_write_cursor=0;
      std::atomic_uint8_t m_read_cursor=0;
      uint32_t m_prob_drop_packet=0;
      bool m_enable =true;
  };
};

