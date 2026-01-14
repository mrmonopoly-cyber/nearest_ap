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

#define IMPLEMENTATION
#include "ac_config.hpp"


extern "C"
{
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"

#include "debug.h"
#include "configblock.h"

#include "motors.h"
}

#define FOR_EACH_ENGINE(engine) for(uint8_t engine=0; engine<NBR_OF_MOTORS; engine++)

void appMain()
{
  using namespace nearest_ap;
  using Node_t = Node<TaskCraziflieSpawner>;
  using Topology = Node_t::Topology;

  Topology topology{num_nodes, default_leader};
  const uint8_t my_id = (configblockGetRadioAddress() & 3);

	vTaskDelay(4000);

  RadioBus bus{};

  auto compute_potential = [my_id]{return 42 + my_id;};

  auto leader_f = [my_id](){
    DEBUG_PRINT("i'm leader: %d\n", my_id);
    motorsSetRatio(MOTOR_M4, MOTORS_TEST_RATIO);
  };

  auto slave_f = [my_id](){
    DEBUG_PRINT("i'm a slave: %d\n", my_id);
    motorsSetRatio(MOTOR_M4, 0);
  };

  if(my_id >= topology.m_num_elements)
  {
    DEBUG_PRINT("id not found in topology: %d\n", my_id);
    while(1)
    {
	    vTaskDelay(4000);
    };
  }


  DEBUG_PRINT("my id: %d, tollerance: %ld\n", my_id, tollerance);
  DEBUG_PRINT("freqs: read: %d, pot: %d, alive: %d\n",
      task_bus_freq, task_pot_alive_freq, task_leader_alive_freq);

  Node node{
      bus,
      TaskCraziflieSpawner{},
      std::move(topology),
      my_id,
      compute_potential,
      leader_f,
      slave_f,
      tollerance,
      task_bus_freq,
      task_pot_alive_freq,
      task_leader_alive_freq 
  };

  DEBUG_PRINT("Starting Drone\n");

  while(1)
  {
    vTaskDelay(99999);
  }
}
