#include <cstdint>


extern "C"
{
#include "radiolink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
}

#include "radio_bus.hpp"

using Msg_t = RadioBus::Msg_t;
using namespace nearest_ap;


static std::queue<Msg_t> g_recv_messages;

static void p2pcallbackHandler(P2PPacket* packet)
{
  // g_recv_messages.emplace(packet->size, packet->rssi, packet->port, packet->data);
}

RadioBus::RadioBus()
{
  p2pRegisterCB(p2pcallbackHandler);
}

Msg_t RadioBus::Read() noexcept
{
  //const TickType_t xDelay = 500 / portTICK_PERIOD_MS;  //from task.h
  const constexpr TickType_t read_delay_ms = 1 / portTICK_PERIOD_MS;

  while (g_recv_messages.empty())
  {
	  vTaskDelay(read_delay_ms);
  };

  Msg_t m{std::move(g_recv_messages.front())};
  g_recv_messages.pop();

  return m;
}

BusStatus_t Write(const Msg_t& msg) noexcept
{
  P2PPacket packet;
  packet.size = static_cast<uint8_t>(msg.m_payload.size());
  packet.rssi =0;
  packet.port = 0x00;

  const pb_byte_t* ptr_data = msg.m_payload.data();

  for(uint8_t i=0; i<msg.m_payload.size();++i)
  {
    packet.data[i] = ptr_data[i];
  }

  radiolinkSendP2PPacketBroadcast(&packet);
  return BusStatus_t::Ok;
}
