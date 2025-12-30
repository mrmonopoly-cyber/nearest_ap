#include "fake_radio_bus.hpp"

#include <iostream>
#include <string>
#include <zmq.hpp>
#include <stdint.h>
#include <stdbool.h>
#include <atomic>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

#include <nearest_ap/logger/logger.hpp>


#define SOCKET_PATH "/tmp/nearest_ap_linux_XX"
#define SOCKET_SUFFIX ".socket"


using namespace nearest_ap;
using socket_t = RadioBus::socket_t;

struct ClientConnectionData
{
  int id;
  P2PCallback p2p_calback;
  zmq::socket_t* sub;
  void* obj;
};

//INFO: private 

static std::atomic_uint8_t _s_id_generator{};

static void _client_connection(ClientConnectionData data)
{
  {
    logger::UserLog<32>log{};
    log.append_msg("node ");
    log.append_msg(data.id);
    log.append_msg(" staring read mex bus");
    static_log(logger::Level::Info, log);
  }

  while (true)
  {
    zmq::message_t msg{};
    try {
      (void)data.sub->recv(msg, zmq::recv_flags::none);
    } catch (const zmq::error_t& e) {
      if (e.num() == ETERM) {
        // context terminated: normal shutdown
        break;
      }
      throw; // real error
    }
    
    if (data.p2p_calback)
    {
      data.p2p_calback(data.obj, reinterpret_cast<P2PPacket*>(msg.data()));
    }
  }

  logger::UserLog<64>log{};
  log.append_msg("node ");
  log.append_msg(data.id);
  log.append_msg(" stopping listening");
  static_log(logger::Level::Warning, log);
}

void RadioBus::enstablis_connection(int num_clients) noexcept
{
  // Connect SUB socket to all other peers
  sub.set(zmq::sockopt::subscribe, "");

  for (Id i=0; i<num_clients; i++)
  {
    std::string pub_addr = "ipc:///tmp/nearest_ap_" + std::to_string(i) + ".bus";

    if (i==m_id) //INFO: if myself
    {
      logger::UserLog<64> log{};
      log.append_msg("client: ");
      log.append_msg(m_id);
      log.append_msg(" skipping client: ");
      log.append_msg(i);
      static_log(logger::Level::Debug, log);

      continue;
    }

    logger::UserLog<128>log{};
    log.append_msg("connecting from client: ");
    log.append_msg(m_id);
    log.append_msg(" to client: ");
    log.append_msg(pub_addr);
    static_log(logger::Level::Debug, log);

    sub.connect(pub_addr);
  }
}

//INFO: public


RadioBus::RadioBus() noexcept :
  m_id(_s_id_generator++),
  pub(ctx, ZMQ_PUB), sub(ctx, ZMQ_SUB)
{
  std::string pub_addr = "ipc:///tmp/nearest_ap_" + std::to_string(m_id)+ ".bus";

  {
    logger::UserLog<128> log{};
    log.append_msg("socket creation: ");
    log.append_msg(pub_addr);
    static_log(logger::Level::Debug, log);
  }

  pub.bind(pub_addr);
}

RadioBus::Id RadioBus::id() const noexcept
{
  return m_id;
}

bool RadioBus::radiolinkSendP2PPacketBroadcast(P2PPacket *p2pp) noexcept
{
  zmq::message_t msg(sizeof(*p2pp));
  std::memcpy(msg.data(), p2pp, sizeof(*p2pp));
  pub.send(msg, zmq::send_flags::none);

  return true;
}

void RadioBus::p2pRegisterCB(void* obj, P2PCallback cb) noexcept
{
  m_obj = obj;
  m_callback = cb;
  ClientConnectionData data{m_id, m_callback, &sub, m_obj};
  receiver = std::thread{_client_connection, std::move(data)};
  receiver.detach();
}

RadioBus::~RadioBus() noexcept{
    running = false;
    ctx.shutdown();        // stops all sockets
    if (receiver.joinable()) receiver.join();
}
