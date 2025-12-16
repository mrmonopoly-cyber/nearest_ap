#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <optional>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

#include <nearest_ap/logger/logger.hpp>

#include "bus.hpp"

#define SOCKET_PATH "/tmp/nearest_ap_linux_XX"
#define SOCKET_SUFFIX ".socket"

using namespace nearest_ap;
using Msg_t = BusLinux_t::Msg_t;
using socket_t = BusLinux_t::socket_t;

struct ClientConnectionData
{
  int id;
  socket_t client_socket;
  std::mutex& lock; 
  std::queue<Msg_t>& msg_queue; 
};

static void _decode(const std::uint8_t raw_buffer[64], Msg_t& msg)
{
    memcpy(&msg.m_msg_size, raw_buffer, sizeof(msg.m_msg_size));
    memcpy(msg.m_payload.data(), raw_buffer + sizeof(msg.m_msg_size), msg.m_payload.size());
}

static void _encode(std::uint8_t raw_buffer[64], const Msg_t& msg)
{
    memcpy(raw_buffer, &msg.m_msg_size, sizeof(msg.m_msg_size));
    memcpy(raw_buffer + sizeof(msg.m_msg_size), msg.m_payload.data(), msg.m_payload.size());
}

static void _client_connection(ClientConnectionData client_socket_data)
{
  ClientConnectionData data = static_cast<ClientConnectionData>(client_socket_data);
  std::uint8_t raw_buffer[64]{};
  Msg_t msg{};

  {
    logger::UserLog<32>log{};
    log.append_msg("node ");
    log.append_msg(client_socket_data.id);
    log.append_msg(" staring read mex bus");
    static_log(logger::Level::Info, log);
  }

  while (true)
  {
    if (data.id==0)
    {
      static_log(logger::Level::Warning, "node 0 waiting mex");
    }
    memset(raw_buffer, 0, sizeof(raw_buffer));
    if(read(data.client_socket, raw_buffer, msg.m_payload.size() + sizeof(msg.m_msg_size))<=0)
    {
      static_log(logger::Level::Debug, "closing connection");
      return;
    }
    _decode(raw_buffer, msg);
    data.lock.lock();
    data.msg_queue.push(std::move(msg));
    data.lock.unlock();
  }

  logger::UserLog<64>log{};
  log.append_msg("node ");
  log.append_msg(data.id);
  log.append_msg(" stopping listening");
  static_log(logger::Level::Warning, log);
}

void BusLinux_t::_socket_setup(void) noexcept
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

void BusLinux_t::enstablis_connection(void) noexcept
{
  const auto id = m_id;
  int written=0;

  for (Id i=0; i<m_max_clients; i++)
  {
    ClientConnectionData data{{},0,m_msg_queue_lock, m_msg_queue};

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

static std::atomic_uint8_t _s_id_generator{};
BusLinux_t::BusLinux_t() noexcept :
m_id(_s_id_generator++),m_socket(0), m_clients(), m_msg_queue(), m_msg_queue_lock(), m_client_connected(0)
{
  _socket_setup();
  std::thread th{BusLinux_t::_Accept, this};

  th.detach();
}

std::optional<Msg_t> BusLinux_t::Read() noexcept
{
  while(!m_msg_queue.empty())
  {
    m_msg_queue_lock.lock();
    Msg_t& m = m_msg_queue.front();
    m_msg_queue.pop();
    m_msg_queue_lock.unlock();

    const constexpr auto failure_percentage = 80;

    const auto percent_failure = rand() % 100;

    if (percent_failure < failure_percentage)
    {
      // logger::UserLog<48>log{};
      // log.append_msg("unlucky case of send with percentage: ");
      // log.append_msg(failure_percentage);
      // static_log(logger::Level::Error,log);
      return {};
    }

    return m;
  }

  return {};
}

BusStatus_t BusLinux_t::Write(const Msg_t& msg) noexcept
{
  const constexpr uint max_tries = 33;

  size_t written =0;
  bool err=false;
  std::uint8_t raw_buffer[64]{};
  const std::uint32_t message_size = msg.m_payload.size() + sizeof(msg.m_msg_size);

  for (std::optional<socket_t>& client : m_clients)
  {
    if (client.has_value())
    {
      uint tries = 0;

      memset(raw_buffer, 0, sizeof(raw_buffer));
      _encode(raw_buffer, msg);

      do
      {
        tries++;
        written = send(*client, raw_buffer, message_size,0);
      }while(written!=message_size && tries < max_tries);

      if (tries >= max_tries)
      {
        err=true;
      }
    }
  }

  return err ? BusStatus_t::UnknowError : BusStatus_t::Ok;
}


void BusLinux_t::_Accept(BusLinux_t* const self) noexcept
{
  ClientConnectionData data{{},0,self->m_msg_queue_lock, self->m_msg_queue};
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
    std::thread client{_client_connection, std::move(data)};
    client.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); //INFO: give time to the thread to copy the data
  }
}
