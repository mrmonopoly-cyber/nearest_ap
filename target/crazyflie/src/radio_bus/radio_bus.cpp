extern "C"
{
#include "radiolink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#include "debug.h"
}

#include <cstdint>

#include "radio_bus.hpp"

using Msg_t = RadioBus::Msg_t;
using namespace nearest_ap;

static struct 
{
  std::array<P2PPacket, UINT8_MAX> msg_queue;
  std::uint8_t free_cell =msg_queue.size();
  std::uint8_t write_cursor;
  std::uint8_t read_cursor;
}g_radio_bus_metadata;


static void p2pcallbackHandler(P2PPacket* packet)
{
  DEBUG_PRINT("recv mex\n");
  if (g_radio_bus_metadata.free_cell > 0)
  {
    g_radio_bus_metadata.msg_queue[g_radio_bus_metadata.write_cursor++] = *packet;
    g_radio_bus_metadata.free_cell--;
  }
}

RadioBus::RadioBus()
{
  p2pRegisterCB(p2pcallbackHandler);
}

std::optional<Msg_t> RadioBus::Read() noexcept
{
  Msg_t m{};
  P2PPacket packet{};

  if(g_radio_bus_metadata.free_cell == g_radio_bus_metadata.msg_queue.size())
  {
    return {};
  }

  packet = g_radio_bus_metadata.msg_queue[g_radio_bus_metadata.read_cursor++];
  g_radio_bus_metadata.free_cell++;

  memcpy(&m.m_msg_size, packet.data, sizeof(m.m_msg_size));
  if (m.m_msg_size < sizeof(packet.data) - m.m_msg_size)
  {
    memcpy(m.m_payload.data(), packet.data + sizeof(m.m_msg_size), m.m_msg_size);
  }
  else
  {
    return {};
  }

  return m;
}

BusStatus_t RadioBus::Write(const Msg_t& msg) noexcept
{
  P2PPacket packet{};
  packet.size = msg.m_msg_size + sizeof(msg.m_msg_size);
  packet.rssi =0;
  packet.port = 0x00;


  memcpy(packet.data, &msg.m_msg_size, sizeof(msg.m_msg_size));
  memcpy(packet.data + sizeof(msg.m_msg_size), msg.m_payload.data(), msg.m_msg_size);

  radiolinkSendP2PPacketBroadcast(&packet);
  return BusStatus_t::Ok;
}
