#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <nearest_ap/nearest_ap.hpp>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "bus/bus.hpp"
#include "logger/logger.hpp"
#include "spawner/spawner.h"
#include "test_output/test_output.hpp"

int main(int argc, char **argv)
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;
  using Topology = Node_t::Topology;

  const auto bus_t_freq = 20;
  const auto pot_t_freq = 700;
  const auto alive_t_freq = 300;

  std::vector<std::unique_ptr<BusLinux_t>> clients{};
  std::vector<std::unique_ptr<Node_t>> drones{};
  std::vector<std::uint32_t> base{};

  char* out_file = nullptr;
  std::uint16_t num_clients = 2;
  auto prob_drop_packet = 0;
  testOut out_test{std::chrono::minutes{5}};
  volatile int stop =20;
  volatile int current_leader =0;


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
      out_file = argv[3];
      out_test.open(out_file);
    case 3:
      prob_drop_packet = std::atoi(argv[2]);
    case 2:
      num_clients= std::atoi(argv[1]);
      break;
  }
#pragma GCC diagnostic pop

  if (out_file)
  {
    std::cout 
      << "num arguments: " << argc
      << " starting simulation with num nodes: " << num_clients
      << " error proability: " << prob_drop_packet
      << " out file: " << out_file
      << std::endl;
  }
  else
  {
    std::cout 
      << "num arguments: " << argc
      << "starting simulation with:" 
      << "num nodes" << num_clients
      << "error proability: " << prob_drop_packet
      << std::endl;
  }

  Topology topology{num_clients, 0};

  clients.reserve(num_clients);
  drones.reserve(num_clients);
  base.resize(num_clients);

  for (uint i=0;i<num_clients; ++i)
  {
    clients.emplace_back(std::make_unique<BusLinux_t>(prob_drop_packet));
  }


  for (auto& client : clients) {
    client->enstablis_connection(num_clients);
  }
  
  for(auto& cell: base)
  {
    cell=num_clients*2;
  }

  bool flag_out_file = false;
  if (out_test.is_ready())
  {
    static_log(nearest_ap::logger::Level::Warning, "out file given");
    flag_out_file = true;
  }
  else
  {
    static_log(nearest_ap::logger::Level::Warning, "out file not given");
    flag_out_file = false;
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
        [&stop,&current_leader,i](){stop=20;current_leader=i;},
        []{},
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
        [&stop,&current_leader,i]{stop--;current_leader=i;}, 
        []{},
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
  
  static_log(logger::Level::Error, "Simulation part 1 ended with success");

  drones[i]->stop();
  clients[i]->disable();

  int tries = 0;
  while(tries < 5000 || current_leader != i-1)
  {
    if (current_leader == i-1)
    {
      tries++;
    }
    else
    {
      tries =0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
  }

  static_log(logger::Level::Error, "Simulation part 2 ended with success");

  if(flag_out_file)out_test.end_test();

  for(auto& drone: drones){
    drone->stop();
  }

  for (auto& bus : clients) {
    bus->disable();
  }

  return 0;
}
