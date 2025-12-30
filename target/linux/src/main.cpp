#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <nearest_ap/nearest_ap.hpp>
#include <sys/types.h>
#include <thread>

#include "bus/bus.hpp"
#include "logger/logger.hpp"
#include "spawner/spawner.h"
#include "test_output/test_output.hpp"

int main(int argc, char **argv)
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;
  using Topology = Node_t::Topology;

  std::uint16_t num_clients = 2;
  auto prob_drop_packet = 0;

  std::vector<std::unique_ptr<BusLinux_t>> clients{};
  std::vector<std::unique_ptr<Node_t>> drones{};
  std::vector<std::uint32_t> base{};

  volatile int stop =20;

  const auto bus_t_freq = 20;
  const auto pot_t_freq = 700;
  const auto alive_t_freq = 100;
  const auto leader_f = [](){};

  testOut out_test{std::chrono::hours{1}};

  LinuxLogger logger{};
  logger::StaticLog{&logger};

  logger::StaticLog::set_level(logger::Level::Debug);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
  switch (argc)
  {
    case 5:
      if (!strcmp(argv[4], "nolog"))
      {
        logger::StaticLog::set_level(nearest_ap::logger::Level::Error);
      }
    case 4:
      out_test.open(argv[3]);
    case 3:
      prob_drop_packet = std::atoi(argv[2]);
    case 2:
      num_clients= std::atoi(argv[1]);
      break;
  }
#pragma GCC diagnostic pop

  clients.reserve(num_clients);
  drones.reserve(num_clients);
  base.resize(num_clients);

  for (uint i=0;i<num_clients; ++i)
  {
    clients.emplace_back(std::make_unique<BusLinux_t>(prob_drop_packet));
  }

  Topology topology{num_clients, 0};

  for (auto& client : clients)
  {
    client->enstablis_connection();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  for(auto& cell: base)
  {
    cell=num_clients*2;
  }

  if (!out_test.is_ready())
  {
    std::cerr << "out file not given" << std::endl;
    return 0;
  }

  out_test.start_test();

  std::uint16_t i=0;
  for (i=0; i<num_clients-1; ++i)
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

  drones.push_back(std::make_unique<Node_t>(
        *clients[i],
        SpawnerLinux_t{},
        std::move(topology),
        i,
        [&base, i]{return base[i] + i;}, 
        [&stop]{stop--;}, 
        0,
        bus_t_freq,
        pot_t_freq + (std::rand() % 100),
        alive_t_freq
        ));


  while(stop > 0){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (out_test.max_time_excedeed())
    {
      stop = 0;
      static_log(logger::Level::Error, "TIMEOUT exceded");
    }
  };

  out_test.end_test();
  return 0;
}
