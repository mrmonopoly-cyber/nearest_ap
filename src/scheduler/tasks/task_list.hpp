#pragma once

#include <cstdint>

namespace  nearest_ap
{
  using TaskId = std::uint8_t;

  enum class InteractibleTask : TaskId
  {
    BUS_READER,
    LEADER_ALIVE,
    POTENTIAL_ELECTION,
  };
}
