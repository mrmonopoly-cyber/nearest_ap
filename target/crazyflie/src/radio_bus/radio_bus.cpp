#include <cstdint>


extern "C"
{
#include "radiolink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
}

#include "radio_bus.hpp"

#include <queue>

using Msg_t = RadioBus::Msg_t;
using namespace nearest_ap;


static std::queue<P2PPacket> g_recv_messages;

static void p2pcallbackHandler(P2PPacket* packet)
{
  g_recv_messages.emplace(*packet);
}

RadioBus::RadioBus()
{
  p2pRegisterCB(p2pcallbackHandler);
}

std::optional<Msg_t> RadioBus::Read() noexcept
{
  if(g_recv_messages.empty())
  {
    return {};
  }

  Msg_t m{};
  P2PPacket& packet = g_recv_messages.front();


  memcpy(&m.m_msg_size, packet.data, sizeof(m.m_msg_size));
  if (sizeof(packet.data) - sizeof(m.m_msg_size) < m.m_msg_size)
  {
    memcpy(m.m_payload.data(), packet.data + sizeof(m.m_msg_size), m.m_msg_size);
  }
  else
  {
    return {};
  }

  g_recv_messages.pop();

  return m;
}

BusStatus_t RadioBus::Write(const Msg_t& msg) noexcept
{
  P2PPacket packet{};
  packet.size = static_cast<uint8_t>(msg.m_payload.size());
  packet.rssi =0;
  packet.port = 0x00;


  memcpy(packet.data, &msg.m_msg_size, sizeof(msg.m_msg_size));
  memcpy(packet.data + sizeof(msg.m_msg_size), msg.m_payload.data(), msg.m_msg_size);

  radiolinkSendP2PPacketBroadcast(&packet);
  return BusStatus_t::Ok;
}
