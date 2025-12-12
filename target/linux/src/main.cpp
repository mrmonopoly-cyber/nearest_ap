#include <array>
#include <chrono>
#include <nearest_ap/nearest_ap.hpp>
#include <optional>
#include <sys/types.h>
#include <thread>

#include "bus/bus.hpp"
#include "spawner/spawner.h"

struct NodeCluster{
  public:

  private:
};


int main()
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;
  // const constexpr uint num_clients = BusLinux_t::m_max_clients;
  const constexpr uint num_clients = 1;

  std::array<BusLinux_t, num_clients> clients{};
  std::array<std::optional<Node_t>, num_clients> drones{};

  auto leader_f = [](){
    return 0;
  };

  for (uint i=0; i<drones.size(); i++)
  {
    drones[i].emplace(clients[i], SpawnerLinux_t{},[]{return 12;}, leader_f, 10);
  }

  for (auto& client : clients)
  {
    client.enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  for (auto& drone : drones)
  {
    drone->async_start();
  }

  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
