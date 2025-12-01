#include "nearest_ap/nearest_ap.h"

int main(void)
{
  using namespace nearest_ap;

  Node<>::SendMex send_f = [](BaseTask<>::BusMex&){return BaseTask<>::BusStatus::Inactive;};
  Node<>::RecvMex recv_f = [](){return BaseTask<>::BusMex{};};
  Node<>::TaskSpawn spawn_f = [](BaseTask<>&){return Tasks<>::SpawnTaskReturn::Error;};

  Node Drone1{send_f, recv_f, spawn_f};

  return 0;
}
