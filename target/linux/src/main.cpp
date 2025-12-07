#include <nearest_ap/nearest_ap.hpp>

#include "bus/bus.h"
#include "spawner/spawner.h"


int main(void)
{
  using namespace nearest_ap;
  using Node_t = Node<SpawnerLinux_t>;

  BusLinux_t bus{};

  Node_t drone{bus, SpawnerLinux_t{}, [](){return 0;}, []{}};

  drone.async_start();

  return 0;
}
