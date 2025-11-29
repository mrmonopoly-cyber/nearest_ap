#pragma once

#include <cstdint>
#include <functional>
#include <array>

#include "tasks/tasks.h"

namespace nearest_ap {

  enum class BusStatus
  {
    Ok,
    QueueFull,
    Inactive,
    UnknowError,
  };

  template<typename BusMex>
  class Node
  {
    public:

      using SendMex = std::function<BusStatus(BusMex)> ;
      using RecvMex = std::function<BusMex()> ;

      Node() = delete;
      explicit Node(const SendMex send_f, const RecvMex recv_f) noexcept;
      void async_start();
      void start();

    private:
      static constexpr uint8_t task_num =3;

      SendMex m_send_f;
      SendMex m_recv_f;
      std::array<Task, task_num> tasks;
  };
}
