#include "nearest_ap/nearest_ap.h"
#include <chrono>
#include <thread>
#include <unistd.h>


int main(void)
{
  using namespace nearest_ap;
  using Node_t = nearest_ap::Node<std::size_t>;

  class BusLinux_t : public Node_t::Bus_t
  {
      Msg Read() const noexcept override
      {
        return Msg{};
      }

      BusStatus_t Write(const Msg&) noexcept override
      {
        return BusStatus_t::Inactive;
      }
  };

  Node_t::TaskSpawn_t spawn_f = [](BaseTask_t&){return SpawnTaskReturn::Error;};
  BusLinux_t bus;
  Node_t::ComputePotF pot_f = [](){return 0;};
  WaitFun_f wait_f = [](Millis_t time){
    std::this_thread::sleep_for(std::chrono::milliseconds{time});
  };

  Node_t Drone1{bus,wait_f, spawn_f, pot_f};

  return 0;
}
