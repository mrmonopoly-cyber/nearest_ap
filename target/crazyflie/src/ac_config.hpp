#ifdef IMPLEMENTATION

#include <stdint.h>

#include <nearest_ap/nearest_ap.hpp>

#include "task_spawner/task_spawner.hpp"

using namespace nearest_ap;

using Node_t = Node<TaskCraziflieSpawner>;

const constexpr uint8_t default_leader =0;
const constexpr uint8_t num_nodes =3;

const constexpr Node_t::Tollercance_t tollerance = 10;

const constexpr Millis_t task_bus_freq  = 200;
const constexpr Millis_t task_pot_alive_freq = 7000;
const constexpr Millis_t task_leader_alive_freq= 1000;

#endif // IMPLEMENTATION
