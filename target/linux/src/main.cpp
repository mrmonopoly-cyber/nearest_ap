#include <nearest_ap/nearest_ap.hpp>

#include "bus/bus.h"
#include "spawner/spawner.h"


int main(void)
{
  using namespace nearest_ap;
  using Node_t = Node<BusLinux_t, SpawnerLinux_t>;

  Node_t drone{SpawnerLinux_t{},BusLinux_t{}, [](){return 0;}};

  drone.async_start();

  return 0;
}
