#include <chrono>
#include <cstring>
#include <iostream>
#include <mutex>
#include <optional>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

#include "bus.hpp"

#define SOCKET_PATH "/tmp/nearest_ap_linux_XX"
#define SOCKET_SUFFIX ".socket"

using namespace nearest_ap;
using Msg_t = BusLinux_t::Msg_t;
using socket_t = BusLinux_t::socket_t;

struct ClientConnectionData
{
  socket_t client_socket;
  std::mutex& lock; 
  std::queue<Msg_t>& msg_queue; 
};

static void _client_connection(ClientConnectionData client_socket_data)
{
  ClientConnectionData data = static_cast<ClientConnectionData>(client_socket_data);
  Msg_t msg{};

  while (true)
  {
    if(read(data.client_socket, msg.m_payload.data(), msg.m_payload.size())<=0)
    {
      std::cout << "closing connection";
      return;
    }
    data.lock.lock();
    data.msg_queue.push(std::move(msg));
    data.lock.unlock();
  }
}

void BusLinux_t::_socket_setup(void) noexcept
{
  struct sockaddr_un sock_addr{
    .sun_family = AF_UNIX,
    .sun_path = SOCKET_PATH SOCKET_SUFFIX,
  };

  const char decimal = '0' + (m_id/10);
  const char unit = '0' + (m_id%10);

  sock_addr.sun_path[sizeof(SOCKET_PATH)-3] = decimal;
  sock_addr.sun_path[sizeof(SOCKET_PATH)-2] = unit;

  unlink(sock_addr.sun_path); //INFO: remove old socket if still present

  std::cout << "socket creation: " << sock_addr.sun_path << std::endl;
  m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (m_socket<0)
  {
    std::cout << "socket creation failed: " << strerror(errno) << std::endl;
    return;
  }

  if(bind(m_socket, reinterpret_cast<const struct sockaddr *>(&sock_addr), sizeof(sock_addr))<0)
  {
    std::cout << "socket bind failed: " << strerror(errno) << std::endl;
    return;
  }

  if(listen(m_socket, m_max_clients)<0)
  {
    std::cout 
      << "set socket listen to : " << m_max_clients 
      << " with written: " << strerror(errno)<< std::endl;
    return;
  }
}

void BusLinux_t::enstablis_connection(void) noexcept
{
  const auto id = m_id;
  int written=0;

  for (Id i=0; i<m_max_clients; i++)
  {
    int sock=0;

    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1  )
    {
      std::cout << "Client: Error on socket() call" << std::endl;
      continue;
    }
    struct sockaddr_un remote
    {
      .sun_family = AF_UNIX,
       .sun_path= SOCKET_PATH SOCKET_SUFFIX
    };

    const char decimal = '0' + (i/10);
    const char unit = '0' + (i%10);

    remote.sun_path[sizeof(SOCKET_PATH)-3] = decimal;
    remote.sun_path[sizeof(SOCKET_PATH)-2] = unit;

    if (i==id || m_clients[i]) //INFO: if myself or already saved
    {
      std::cout 
        << "client: " << id
        << " skipping client: " << i
        << std::endl;
      continue;
    }

    if((written=connect(sock, reinterpret_cast<const struct sockaddr *>(&remote), sizeof(remote))<0))
    {
      std::cout 
        << "error connecting from client: " << id 
        << " to client: " << remote.sun_path 
        << " skipping client"
        << std::endl;
      continue;
    }

    std::cout 
      << "connection ok from client: " << id 
      << " to client: " << remote.sun_path 
      << std::endl;
    m_clients[m_client_connected++] = sock;
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
    return m;
  }

  return {};
}

BusStatus_t BusLinux_t::Write(const Msg_t& msg) noexcept
{
  size_t written =0;
  bool err=false;
  const constexpr uint max_tries = 33;

  for (socket_t& client : m_clients)
  {
    uint tries = 0;

    do
    {
      tries++;
      written = write(client, msg.m_payload.data(), msg.m_payload.size());
    }while(written!=msg.m_payload.size() && tries < max_tries);

    if (tries >= max_tries)
    {
      err=true;
    }
  }

  return err ? BusStatus_t::UnknowError : BusStatus_t::Ok;
}

void BusLinux_t::_Accept(BusLinux_t* const self) noexcept
{
  ClientConnectionData data{0,self->m_msg_queue_lock, self->m_msg_queue};
  sockaddr_un remote{};
  socklen_t size = sizeof(remote);

  while (true)
  {
    data.client_socket = accept(self->m_socket,reinterpret_cast<sockaddr*>(&remote), &size);
    if (data.client_socket<0)
    {
      std::cout
        << "error accepting connection: "
        << " bus: " << self->m_id 
        << "from: " << remote.sun_path
        << std::endl;
      continue;
    }
    self->m_clients[self->m_client_connected++] = data.client_socket;
    std::thread client{_client_connection, std::move(data)};
    client.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); //INFO: give time to the thread to copy the data
  }
}
