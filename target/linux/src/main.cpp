#include <chrono>
#include <cstdint>
#include <memory>
#include <nearest_ap/nearest_ap.hpp>
#include <sys/types.h>
#include <thread>

#include "bus/bus.hpp"
#include "logger/logger.hpp"
#include "spawner/spawner.h"

int main(int argc, char **argv)
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;
  using Topology = Node_t::Topology;

  LinuxLogger logger{};
  logger::StaticLog{&logger};
  auto leader_f = [](){};

  logger.setLevel(logger::Level::Info);

  std::uint16_t num_clients = 2;

  if (argc >=2)
  {
    num_clients= std::atoi(argv[1]);
  }


  std::vector<std::unique_ptr<BusLinux_t>> clients{};
  std::vector<std::unique_ptr<Node_t>> drones{};
  std::vector<Node_t::VirtualId_t> user_ids{};

  user_ids.reserve(num_clients);
  clients.reserve(num_clients);
  drones.reserve(num_clients);

  for (uint i=0;i<num_clients; ++i)
  {
    clients.emplace_back(std::make_unique<BusLinux_t>());
    user_ids.emplace_back(i);
  }

  Topology topology{user_ids, 1};

  for (auto& client : clients)
  {
    client->enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  auto bus_t_freq = 10;
  auto pot_t_freq = 1000;
  auto alive_t_freq = 30;

  for (std::uint16_t i=0; i<num_clients; ++i)
  {
    drones.push_back(std::make_unique<Node_t>(
        *clients[i],
        SpawnerLinux_t{},
        topology,
        i,
        []{return 12;}, 
        leader_f, 
        10,
        bus_t_freq,
        pot_t_freq,
        alive_t_freq));
  }

  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
