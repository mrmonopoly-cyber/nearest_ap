#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <cerrno>
#include <unistd.h>

#include "bus.h"

#define SOCKET_PATH "/tmp/nearest_ap_linux.socket"

using namespace nearest_ap;
using Msg_t = BusLinux_t::Msg_t;

static void _socket_setup(int& m_socket, const int max_connections)
{
  struct sockaddr_un sock_addr{
    .sun_family = AF_UNIX,
    .sun_path = SOCKET_PATH,
  };

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

BusLinux_t::BusLinux_t()
{
  int max_connections = 5;
  _socket_setup(m_socket, max_connections);
}

BusLinux_t::BusLinux_t(const int max_connections)
{
  _socket_setup(m_socket, max_connections);
}

Msg_t BusLinux_t::Read() const noexcept
{
  Msg_t msg{};
  read(m_socket, msg.m_payload.data(), msg.m_payload.size());
  return msg;
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
