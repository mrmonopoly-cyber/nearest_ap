#include <iostream>
#include <optional>
#include <sys/socket.h>
#include <sys/un.h>
#include <cerrno>
#include <unistd.h>

#include "bus.hpp"

#define SOCKET_PATH "/tmp/nearest_ap_linux.socket"

using namespace nearest_ap;
using Msg_t = BusLinux_t::Msg_t;

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
m_socket(0), m_client_connected(0), m_clients()
{
  const constexpr int max_connections = 5;
  _socket_setup(m_socket, max_connections);
}

BusLinux_t::BusLinux_t(const int max_connections) noexcept :
m_socket(0), m_client_connected(0), m_clients()
{
  _socket_setup(m_socket, max_connections);
}

void BusLinux_t::Accept_connections() noexcept
{
  int new_client =0;

  while (true)
  {
    new_client = accept(m_socket, NULL, NULL);
    m_clients[m_client_connected++] = new_client;
    //TODO: start read thread for this client
  }
}

std::optional<Msg_t> BusLinux_t::Read() noexcept
{
  while(!m_msg_queue.empty())
  {
  
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
