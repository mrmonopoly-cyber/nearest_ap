#include <array>
#include <chrono>
#include <nearest_ap/nearest_ap.hpp>
#include <thread>

#include "bus/bus.hpp"
#include "spawner/spawner.h"


int main(void)
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;

  std::array<BusLinux_t, BusLinux_t::m_max_clients> clients{};

  for (uint i=0 ; i<BusLinux_t::m_max_clients;i++)
  {
    clients[i].enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  Node_t drone{clients[0], SpawnerLinux_t{}, [](){return 0;}, []{}};

  drone.async_start();

  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
