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

void appMain()
{
  using namespace nearest_ap;
  using Node_t = Node<TaskCraziflieSpawner>;
  using Topology = Node_t::Topology;

  const constexpr uint8_t default_leader =0;
  Topology topology{4, default_leader};
  const uint8_t my_id = ((uint8_t)configblockGetRadioAddress()) & 0x0002;

	vTaskDelay(4000);

  RadioBus bus{};

  Node_t::Tollercance_t tollerance = 10;
  auto compute_potential = []{return 42;};

  auto leader_f = [my_id](){
    char buffer[32] = "i'm leader: ";
    DEBUG_PRINT("%s%d\n",buffer, my_id);
  };

  if(my_id >= topology.m_num_elements)
  {
    DEBUG_PRINT("id not found in topology: %d\n", my_id);
    while(1){};
  }


  DEBUG_PRINT("my (id:index): %d, %d\n", my_id, my_id);

  Node node{
      bus,
      TaskCraziflieSpawner{},
      std::move(topology),
      my_id,
      compute_potential,
      leader_f,
      tollerance,
      200,
      7000,
      1000
  };

  DEBUG_PRINT("Starting Drone\n");

  while(1)
  {
    vTaskDelay(99999);
  }

}
