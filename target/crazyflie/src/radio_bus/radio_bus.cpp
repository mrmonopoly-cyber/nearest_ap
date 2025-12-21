extern "C"
{
#include "radiolink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#include "debug.h"
}

#include <cstdint>
#include <atomic>

#include "radio_bus.hpp"

using Msg_t = RadioBus::Msg_t;
using namespace nearest_ap;

static struct 
{
  std::array<P2PPacket, UINT8_MAX> msg_queue;
  std::atomic_uint8_t free_cell =msg_queue.size();
  std::atomic_uint8_t write_cursor;
  std::atomic_uint8_t read_cursor;
}g_radio_bus_metadata;


static void p2pcallbackHandler(P2PPacket* packet)
{
  if (g_radio_bus_metadata.free_cell > 0)
  {
    g_radio_bus_metadata.msg_queue[g_radio_bus_metadata.write_cursor++] = *packet;
    g_radio_bus_metadata.free_cell--;
  }
}

static inline void _decode(const P2PPacket& raw_packet, Msg_t& msg)
{
  msg.m_msg_size = raw_packet.size;
  memcpy(msg.m_payload.data(), raw_packet.data, raw_packet.size);
}

static inline void _encode(P2PPacket& raw_packet, const Msg_t& msg)
{
  raw_packet.size  = msg.m_msg_size;
  memcpy(raw_packet.data, msg.m_payload.data(), msg.m_msg_size);
}

RadioBus::RadioBus()
{
  p2pRegisterCB(p2pcallbackHandler);
}

uint8_t RadioBus::_next(const uint8_t v) const noexcept
{
  return (v+1) % s_queue_size;
}

std::optional<Msg_t> RadioBus::Read() noexcept
{
  Msg_t m{};
  P2PPacket packet{};

  const uint8_t read_cursor = g_radio_bus_metadata.read_cursor.load();

  if (read_cursor != g_radio_bus_metadata.write_cursor.load())
  {
    packet = g_radio_bus_metadata.msg_queue[read_cursor];
    g_radio_bus_metadata.read_cursor.store(_next(read_cursor));
    _decode(packet, m);

    return m;
  }

  return std::nullopt;
}


BusStatus_t RadioBus::Write(const Msg_t& msg) noexcept
{
  P2PPacket packet{};
  packet.size = msg.m_msg_size;
  packet.rssi =0;
  packet.port = 0x00;

  _encode(packet, msg);

  radiolinkSendP2PPacketBroadcast(&packet);
  return BusStatus_t::Ok;
}
