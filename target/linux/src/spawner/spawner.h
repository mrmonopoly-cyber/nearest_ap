#pragma once

#include <iostream>

#include <nearest_ap/nearest_ap.hpp>
#include <sys/types.h>

namespace nearest_ap
{
class SpawnerLinux_t : public Spawner_t
  {
    public:
      SpawnerLinux_t()
      {
        std::cout << "Spawner creation " << std::endl;
      }

      SpawnerLinux_t(const SpawnerLinux_t& ) =delete;
      SpawnerLinux_t operator=(const SpawnerLinux_t& ) =delete;

      SpawnerLinux_t(SpawnerLinux_t&& ) = default;
      SpawnerLinux_t& operator=(SpawnerLinux_t&& ) = default;

      void start_task(BaseTask_t* t) override
      {
        std::cout
          << "staring task: " << t->id()
          << std::endl;

      }

      void start_task(BaseTask_t* t, Millis_t f) override
      {
        std::cout
          << "staring task: " << t->id()
          << " with freq: " << f
          << std::endl;
      }
  };
}
