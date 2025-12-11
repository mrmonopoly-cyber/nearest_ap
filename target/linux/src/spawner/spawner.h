#pragma once

#include <nearest_ap/nearest_ap.hpp>

namespace nearest_ap
{
class SpawnerLinux_t : public Spawner_t
  {
    public:
      explicit SpawnerLinux_t(){}

      SpawnerLinux_t(const SpawnerLinux_t& ) =delete;
      SpawnerLinux_t operator=(const SpawnerLinux_t& ) =delete;

      SpawnerLinux_t(SpawnerLinux_t&& ) = default;
      SpawnerLinux_t& operator=(SpawnerLinux_t&& ) = default;

      void start_task(BaseTask_t* t) noexcept override;
      void start_task(BaseTask_t* t, Millis_t f) noexcept override;
  };
}
