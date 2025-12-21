#include "fake_radio_bus.hpp"

#include <cstdint>
#include <stdint.h>
#include <stdbool.h>
#include <atomic>
#include <array>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

#include <nearest_ap/logger/logger.hpp>


#define SOCKET_PATH "/tmp/nearest_ap_linux_XX"
#define SOCKET_SUFFIX ".socket"


using namespace nearest_ap;
using socket_t = RadioBus::socket_t;

struct ClientConnectionData
{
  int id;
  socket_t client_socket;
  P2PCallback p2p_calback;
  void* obj;
};

//INFO: private 

static std::atomic_uint8_t _s_id_generator{};

static void _client_connection(ClientConnectionData data)
{
  P2PPacket msg{};

  {
    logger::UserLog<32>log{};
    log.append_msg("node ");
    log.append_msg(data.id);
    log.append_msg(" staring read mex bus");
    static_log(logger::Level::Info, log);
  }

  while (true)
  {
    if (data.id==0)
    {
      static_log(logger::Level::Warning, "node 0 waiting mex");
    }
    if(read(data.client_socket, &msg, sizeof(msg))<=0)
    {
      static_log(logger::Level::Debug, "closing connection");
      return;
    }
    if (data.p2p_calback)
    {
      data.p2p_calback(data.obj, &msg);
    }
  }

  logger::UserLog<64>log{};
  log.append_msg("node ");
  log.append_msg(data.id);
  log.append_msg(" stopping listening");
  static_log(logger::Level::Warning, log);
}

void RadioBus::_socket_setup(void) noexcept
{
  srand(time(NULL));
  struct sockaddr_un sock_addr{
    .sun_family = AF_UNIX,
    .sun_path = SOCKET_PATH SOCKET_SUFFIX,
  };

  const char decimal = '0' + (m_id/10);
  const char unit = '0' + (m_id%10);

  sock_addr.sun_path[sizeof(SOCKET_PATH)-3] = decimal;
  sock_addr.sun_path[sizeof(SOCKET_PATH)-2] = unit;

  unlink(sock_addr.sun_path); //INFO: remove old socket if still present

  {
    logger::UserLog<32 + sizeof(sock_addr.sun_path)> log{};
    log.append_msg("socket creation: ");
    log.append_msg(std::string_view(sock_addr.sun_path));
    static_log(logger::Level::Debug, log);
  }

  m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (m_socket<0)
  {
    logger::UserLog<64 + sizeof(sock_addr.sun_path)> log{};
    log.append_msg("socket creation: ");
    log.append_msg(std::string_view(sock_addr.sun_path));
    log.append_msg(", socket creation failed: ");
    log.append_msg(strerror(errno));
    static_log(logger::Level::Debug, log);
    return;
  }

  if(bind(m_socket, reinterpret_cast<const struct sockaddr *>(&sock_addr), sizeof(sock_addr))<0)
  {
    logger::UserLog<32> log{};
    log.append_msg("socket bind failed: ");
    log.append_msg(strerror(errno));
    static_log(logger::Level::Debug, log);
    return;
  }

  if(listen(m_socket, m_max_clients)<0)
  {
    logger::UserLog<64> log{};
    log.append_msg("set socket listen to : ");
    log.append_msg(static_cast<uint32_t>(m_max_clients));
    log.append_msg("with written: ");
    log.append_msg(strerror(errno));
    static_log(logger::Level::Debug, log);

    return;
  }
}

void RadioBus::_Accept(RadioBus* const self) noexcept
{
  ClientConnectionData data{{},0, self->m_callback, self->m_obj};
  sockaddr_un remote{};
  socklen_t size = sizeof(remote);

  while (true)
  {
    data.client_socket = accept(self->m_socket,reinterpret_cast<sockaddr*>(&remote), &size);
    if (data.client_socket<0)
    {
      logger::UserLog<64 + sizeof(remote.sun_path)>log{};
      log.append_msg("error accepting connection. bus: ");
      log.append_msg(self->m_id);
      log.append_msg(" from: ");
      log.append_msg(std::string_view(remote.sun_path));
      static_log(logger::Level::Debug, log);

      continue;
    }
    self->m_clients[self->m_client_connected++] = data.client_socket;
    data.id = self->m_id;
    std::thread client{_client_connection, data};
    client.detach();
  }
}

void RadioBus::enstablis_connection(void) noexcept
{
  const auto id = m_id;
  int written=0;

  for (Id i=0; i<m_max_clients; i++)
  {
    ClientConnectionData data{{},0,m_callback, m_obj};

    if( (data.client_socket= socket(AF_UNIX, SOCK_STREAM, 0)) == -1  )
    {
      logger::UserLog<64>log{};
      log.append_msg("Client: Error on socket() call: ");
      log.append_msg(strerror(errno));
      static_log(logger::Level::Debug, log);
      continue;
    }
    struct sockaddr_un remote
    {
      .sun_family = AF_UNIX,
       .sun_path= SOCKET_PATH SOCKET_SUFFIX
    };

    data.id = i;
    const char decimal = '0' + (i/10);
    const char unit = '0' + (i%10);

    remote.sun_path[sizeof(SOCKET_PATH)-3] = decimal;
    remote.sun_path[sizeof(SOCKET_PATH)-2] = unit;

    if (i==id || m_clients[i]) //INFO: if myself or already saved
    {
      logger::UserLog<64> log{};
      log.append_msg("client: ");
      log.append_msg(id);
      log.append_msg(" skipping client: ");
      log.append_msg(i);
      static_log(logger::Level::Debug, log);

      continue;
    }

    if((written=connect(data.client_socket, reinterpret_cast<const struct sockaddr *>(&remote), sizeof(remote))<0))
    {
      logger::UserLog<64 + sizeof(remote.sun_path)>log{};
      log.append_msg("error connecting from client: ");
      log.append_msg(id);
      log.append_msg(" skipping client");
      log.append_msg(std::string_view(remote.sun_path));
      static_log(logger::Level::Debug, log);

      continue;
    }

    logger::UserLog<64 + sizeof(remote.sun_path)>log{};
    log.append_msg("connection ok from client: ");
    log.append_msg(id);
    log.append_msg(" to client: ");
    log.append_msg(std::string_view(remote.sun_path));
    static_log(logger::Level::Debug, log);

    std::thread client{_client_connection, std::move(data)};
    client.detach();

    m_clients[m_client_connected++] = data.client_socket;
  }
}

//INFO: public


RadioBus::RadioBus() noexcept : m_id(_s_id_generator++)
{
  _socket_setup();
  std::thread th{RadioBus::_Accept, this};

  th.detach();

}

RadioBus::Id RadioBus::id() const noexcept
{
  return m_id;
}

bool RadioBus::radiolinkSendP2PPacketBroadcast(P2PPacket *p2pp) noexcept
{
  for (std::optional<socket_t>& client : m_clients)
  {
    if (client.has_value())
    {
      send(*client, p2pp, sizeof(*p2pp),0);
    }
  }

  return true;
}

void RadioBus::p2pRegisterCB(void* obj, P2PCallback cb) noexcept
{
  m_obj = obj;
  m_callback = cb;
}
