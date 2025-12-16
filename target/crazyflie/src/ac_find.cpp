/**
 * ,---------,       ____  _ __
 * |  ,-^-,  |      / __ )(_) /_______________ _____  ___
 * | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * | / ,--´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *    +------`   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2025 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <cstdint>
#include <stdint.h>
#include <stdbool.h>

#include <nearest_ap/nearest_ap.hpp>

#include "task_spawner/task_spawner.hpp"
#include "radio_bus/radio_bus.hpp"


extern "C"
{
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"

#include "debug.h"
#include "token_ring.h"
}

#define NETWORK_TOPOLOGY {0, 1, 2, 3} // Maximum size of network is 20 by default

void appMain()
{
  using namespace nearest_ap;
  using Node_t = Node<TaskCraziflieSpawner>;
  using Topology = Node_t::Topology;

  const constexpr uint8_t default_leader =0;

  const Topology topology{NETWORK_TOPOLOGY, default_leader};
  RadioBus bus{};
  Node_t::Tollercance_t tollerance = 10;
  auto leader_f = [](){};
  auto compute_potential = []{return 42;};
  std::uint16_t user_index =0;
  uint8_t my_id = dtrGetSelfId();

  for (uint8_t i=0;i<topology.m_elements.size();i++)
  {
    if (topology.m_elements[i]==my_id)
    {
      my_id =i;
      break;
    }
  }

  Node node{bus, TaskCraziflieSpawner{}, topology,
      user_index, compute_potential, leader_f, tollerance};

  DEBUG_PRINT("Starting Drone\n");
}
