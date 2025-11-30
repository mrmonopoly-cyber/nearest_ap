#include "nearest_ap/nearest_ap.h"

struct BusMex{
};

int main(void)
{
  using namespace nearest_ap;

  auto send_f = [](BusMex&){return BaseTask<BusMex>::BusStatus::Inactive;};
  auto recv_f = [](){return BusMex{};};

  Node<BusMex, 3> Drone{send_f, recv_f};

  return 0;
}
