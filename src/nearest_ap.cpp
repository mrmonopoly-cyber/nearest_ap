#include "nearest_ap.h"

using namespace nearest_ap;


template<typename BusMex, int num_nodes>
Node<BusMex, num_nodes>::Node(const SendMex send_f, const RecvMex recv_f) noexcept
: m_send_f(send_f), m_recv_f(send_f)
{
}

template<typename BusMex, int num_nodes>
void Node<BusMex, num_nodes>::async_start()
{
  for (auto& task : m_tasks) {
    auto err = m_task_spawn(*task);
    while(err ==SpawnTaskReturn::Error)
    {
      err = m_task_spawn(*task);
      m_debug_print(err);
    }
  }
}
