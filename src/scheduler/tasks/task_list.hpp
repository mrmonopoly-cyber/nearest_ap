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

    TASK_COUNT //INFO: DO not use leave it at the end
  };

  enum class NonInteractibleTask : TaskId
  {
    EVENT_LOOP_TASK = static_cast<TaskId>(InteractibleTask::TASK_COUNT)
  };
}
