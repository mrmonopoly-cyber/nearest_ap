#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <array>
#include <memory>

#include "tasks/tasks.h"
#include "internal/potential_info/potential_info.h"

namespace nearest_ap {

  enum class BusStatus
  {
    Ok,
    QueueFull,
    Inactive,
    UnknowError,
  };

  enum class SpawnTaskReturn
  {
    Ok,
    Error,
  };


  template<typename BusMex, int num_nodes>
  class Node
  {
    public:
      using DebugPrint = std::function<void(SpawnTaskReturn)>;
      using SendMex = std::function<BusStatus(BusMex)> ;
      using RecvMex = std::function<BusMex()> ;
      using taskSpawn = std::function<SpawnTaskReturn(Task&)>;

      Node() = delete;

      explicit Node(const SendMex send_f, const RecvMex recv_f) noexcept;

      explicit Node(
          const SendMex send_f,
          const RecvMex recv_f,
          const DebugPrint debug_print_f) noexcept;

      void async_start(void);

    private:
      static constexpr uint8_t m_task_num =3;
      taskSpawn m_task_spawn;
      SendMex m_send_f;
      SendMex m_recv_f;
      DebugPrint m_debug_print = [](SpawnTaskReturn){};

      std::array<std::unique_ptr<Task>, m_task_num> m_tasks;
  };
}
