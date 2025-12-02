#include "nearest_ap/nearest_ap.h"


int main(void)
{
  using namespace nearest_ap;
  using Node_t = nearest_ap::Node<std::size_t>;

  class BusLinux_t : public Node_t::Bus_t
  {
      Msg Read() noexcept override
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

  Node_t Drone1{bus,spawn_f};

  return 0;
}
