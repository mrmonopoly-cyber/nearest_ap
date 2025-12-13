#include <array>
#include <chrono>
#include <nearest_ap/nearest_ap.hpp>
#include <optional>
#include <sys/types.h>
#include <thread>

#include "bus/bus.hpp"
#include "logger/logger.hpp"
#include "spawner/spawner.h"

int main()
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;
  using Topology = Node_t::Topology;

  const constexpr uint num_clients = 2;

  LinuxLogger logger{};
  logger::StaticLog{&logger};
  auto leader_f = [](){};

  std::vector<BusLinux_t> clients{num_clients};
  std::vector<std::optional<Node_t>> drones{num_clients};

  Topology topology{{0, 1}, 1};

  for (auto& client : clients)
  {
    client.enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  for (uint i=0; i<drones.size(); i++)
  {
    drones[i].emplace(
        clients[i],
        SpawnerLinux_t{},
        topology,
        i,
        []{return 12;}, 
        leader_f, 
        10);
  }

  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
