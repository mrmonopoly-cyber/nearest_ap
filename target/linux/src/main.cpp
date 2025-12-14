#include <chrono>
#include <cstdint>
#include <cstdlib>
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

  logger::StaticLog::set_level(logger::Level::Debug);

  std::uint16_t num_clients = 2;
  auto leader = num_clients-1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
  switch (argc)
  {
    case 3:
      leader = std::atoi(argv[2]);
    case 2:
      num_clients= std::atoi(argv[1]);
    default:
      break;
  }
#pragma GCC diagnostic pop

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

  Topology topology{user_ids, static_cast<uint32_t>(leader)};

  for (auto& client : clients)
  {
    client->enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  auto bus_t_freq = 1000;
  auto pot_t_freq = 5000;
  auto alive_t_freq = 1000;

  (void) bus_t_freq;
  (void) pot_t_freq;
  (void) alive_t_freq;

  for (std::uint16_t i=0; i<num_clients; ++i)
  {
    drones.push_back(std::make_unique<Node_t>(
        *clients[i],
        SpawnerLinux_t{},
        topology,
        i,
        [i]{return 12 + i;}, 
        leader_f, 
        0,
        bus_t_freq,
        pot_t_freq,
        alive_t_freq
        ));
  }

  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
