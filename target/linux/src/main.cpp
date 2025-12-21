#include <array>
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

  clients.reserve(num_clients);
  drones.reserve(num_clients);

  for (uint i=0;i<num_clients; ++i)
  {
    clients.emplace_back(std::make_unique<BusLinux_t>());
  }

  Topology topology{num_clients, static_cast<uint32_t>(leader)};

  for (auto& client : clients)
  {
    client->enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  auto bus_t_freq = 200;
  auto pot_t_freq = 7000;
  auto alive_t_freq = 1000;

  (void) bus_t_freq;
  (void) pot_t_freq;
  (void) alive_t_freq;

  int j=0;
  std::vector<std::uint32_t> base{};
  base.resize(num_clients);

  for(auto& cell: base)
  {
    cell=num_clients*2;
  }

  drones.push_back(std::make_unique<Node_t>(
        *clients[0],
        SpawnerLinux_t{},
        std::move(topology),
        0,
        [&base, &j]{return base[0] + j;}, 
        leader_f, 
        0,
        bus_t_freq + (std::rand() % 10),
        pot_t_freq + (std::rand() % 100),
        alive_t_freq
        ));

  for (std::uint16_t i=1; i<num_clients; ++i)
  {
    drones.push_back(std::make_unique<Node_t>(
        *clients[i],
        SpawnerLinux_t{},
        std::move(topology),
        i,
        [&base, i]{return base[i] + i;}, 
        leader_f, 
        0,
        bus_t_freq,
        pot_t_freq + (std::rand() % 100),
        alive_t_freq
        ));
  }

  // std::this_thread::sleep_for(std::chrono::seconds(70));
  // static_log(nearest_ap::logger::Level::Warning, "ALL NODES WILL DECREASE THEIR POTENTIAL");
  // base[0] = 0;
  //
  // for (int i=1; i<num_clients ; i++)
  // {
  //   base[i] = num_clients*2 - 2*i;
  // }
  //
  std::this_thread::sleep_for(std::chrono::hours(99));

  return 0;
}
