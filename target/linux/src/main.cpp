#include "nearest_ap/nearest_ap.h"

int main(void)
{
  using namespace nearest_ap;
  using Node_t = nearest_ap::Node<std::size_t>;


  Node_t::SendMex_t send_f = [](Node_t::Tasks_t::BusMex_t&){return BusStatus_t::Inactive;};
  Node_t::RecvMex_t recv_f = [](){return Node_t::Tasks_t::BusMex_t{};};
  Node_t::TaskSpawn_t spawn_f = [](Node_t::Tasks_t::BaseTask_t&){return SpawnTaskReturn::Error;};

  Node_t Drone1{send_f, recv_f, spawn_f};

  return 0;
}
