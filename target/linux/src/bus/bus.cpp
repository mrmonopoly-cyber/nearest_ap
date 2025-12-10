#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

#include "bus.hpp"

#define SOCKET_PATH "/tmp/nearest_ap_linux.socket"

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
      return;
    }
    data.lock.lock();
    data.msg_queue.push(std::move(msg));
    data.lock.unlock();
  }
}

static void _socket_setup(int& m_socket, const int max_connections)
{
  struct sockaddr_un sock_addr{
    .sun_family = AF_UNIX,
    .sun_path = SOCKET_PATH,
  };

  unlink(SOCKET_PATH); //INFO: remove old socket if still present

  std::cout << "bus creation for local comunication" << std::endl;

  std::cout << "socket creation" << std::endl;
  m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (m_socket<0)
  {
    std::cout << "socket creation failed: " << errno << std::endl;
  }

  if(bind(m_socket, reinterpret_cast<const struct sockaddr *>(&sock_addr), sizeof(sock_addr))<0)
  {
    std::cout << "socket bind failed: " << errno << std::endl;
  }

  if(listen(m_socket, max_connections)<0)
  {
    std::cout << "set socket listen to : " << max_connections << "with err: " << errno<< std::endl;
  }
}

BusLinux_t::BusLinux_t() noexcept :
m_socket(0), m_clients(), m_msg_queue(), m_msg_queue_lock(), m_client_connected(0)
{
  const constexpr int max_connections = 5;
  _socket_setup(m_socket, max_connections);
}

BusLinux_t::BusLinux_t(const int max_connections) noexcept :
m_socket(0), m_clients(), m_msg_queue(), m_msg_queue_lock(), m_client_connected(0)
{
  _socket_setup(m_socket, max_connections);
}

void BusLinux_t::Accept_connections() noexcept
{
  std::thread th{BusLinux_t::_Accept, this};
  th.detach();
}

std::optional<Msg_t> BusLinux_t::Read() noexcept
{
  while(!m_msg_queue.empty())
  {
    Msg_t& m = m_msg_queue.front();
    m_msg_queue.pop();
    return m;
  }

  return {};
}

BusStatus_t BusLinux_t::Write(const Msg_t& msg) noexcept
{
  size_t byte_wrote = write(m_socket, msg.m_payload.data(), msg.m_payload.size());
  if(byte_wrote != msg.m_payload.size())
  {
    return BusStatus_t::UnknowError;
  }

  return  BusStatus_t::Ok;
}

void BusLinux_t::_Accept(BusLinux_t* const self) noexcept
{
  ClientConnectionData data{0,self->m_msg_queue_lock, self->m_msg_queue};

  while (true)
  {
    data.client_socket = accept(self->m_socket, NULL, NULL);
    self->m_clients[self->m_client_connected++] = data.client_socket;
    std::thread client{_client_connection, std::move(data)};
    client.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); //INFO: give time to the thread to copy the data
  }
}
