#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <optional>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <nearest_ap/logger/logger.hpp>

#include "bus.hpp"

#include "fake_radio_bus/fake_radio_bus.hpp"


using namespace nearest_ap;
using Msg_t = BusLinux_t::Msg_t;

uint8_t BusLinux_t::_next(const uint8_t v) const noexcept
{
  return (v+1) % s_queue_size;
}


void BusLinux_t::p2pcallbackHandler(P2PPacket* packet) noexcept
{
  const uint8_t write_cursor = m_write_cursor.load();

  if (static_cast<uint32_t>(rand() % 100) < m_prob_drop_packet)
  {
    return;
  }

  if (_next(write_cursor) != m_read_cursor.load())
  {
    m_msg_queue[write_cursor] = *packet;
    m_write_cursor.store(_next(write_cursor));
#ifdef BUS_LOG
    logger::UserLog<1000>log;
    log.append_msg("node: ");
    log.append_msg(m_radio_bus.id());
    log.append_msg(" saving in: ");
    log.append_msg(write_cursor);
    log.append_msg("new recv mex with size: ");
    log.append_msg(static_cast<uint32_t>(packet->size));
    log.append_msg(" and data: ");
    for(int i=0;i<packet->size;i++)
    {
      log.append_msg("data[");
      log.append_msg(i);
      log.append_msg("]=");
      log.append_msg(packet->data[i]);
      log.append_msg(",");
    }
    static_log(logger::Level::Warning, log);
#endif // BUS_LOG
  }
  else
  {
    static_log(logger::Level::Error, "reject new mex");
  }
}

static inline void _decode(const P2PPacket& raw_packet, Msg_t& msg)
{
  msg.m_msg_size = raw_packet.size;
  memcpy(msg.m_payload.data(), raw_packet.full_data.data, raw_packet.size);
}

static inline void _encode(P2PPacket& raw_packet, const Msg_t& msg)
{
  raw_packet.size  = msg.m_msg_size;
  memcpy(raw_packet.full_data.data, msg.m_payload.data(), msg.m_msg_size);
}

BusLinux_t::BusLinux_t(const uint32_t prob) noexcept :
m_radio_bus(),m_prob_drop_packet(prob)
{
  srand(time(NULL));
  m_radio_bus.p2pRegisterCB(this, _p2pcallbackHandler);
}


std::optional<Msg_t> BusLinux_t::Read() noexcept
{
  Msg_t m{};
  P2PPacket packet{};

  const uint8_t read_cursor = m_read_cursor.load();

  if (read_cursor != m_write_cursor.load())
  {

#ifdef BUS_LOG
    logger::UserLog<1000>log;
    log.append_msg("node: ");
    log.append_msg(m_radio_bus.id());
    log.append_msg(" reading in: ");
    log.append_msg(read_cursor);
    log.append_msg("mex with size: ");
    log.append_msg(static_cast<uint32_t>(packet.size));
    log.append_msg(" and data: ");
    for(int i=0;i<packet.size;i++)
    {
      log.append_msg("data[");
      log.append_msg(i);
      log.append_msg("]=");
      log.append_msg(packet.data[i]);
      log.append_msg(",");
    }
    static_log(logger::Level::Warning, log);
#endif // BUS_LOG

    packet = m_msg_queue[read_cursor];
    m_read_cursor.store(_next(read_cursor));
    _decode(packet, m);

    return m;
  }

  return std::nullopt;
}


BusStatus_t BusLinux_t::Write(const Msg_t& msg) noexcept
{
  P2PPacket packet{};
  packet.size = msg.m_msg_size;
  packet.rssi =0;
  packet.full_data.port = 0x00;

  _encode(packet, msg);

  m_radio_bus.radiolinkSendP2PPacketBroadcast(&packet);
  return BusStatus_t::Ok;
}
