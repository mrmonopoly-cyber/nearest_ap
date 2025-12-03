#include <unistd.h>
#include <iostream>

#include "nearest_ap/nearest_ap.h"

using namespace nearest_ap;

class BusLinux_t : public Bus<std::size_t>
{
  public:
    BusLinux_t()
    {
      std::cout << "bus creation" << std::endl;
    }

    BusLinux_t(const BusLinux_t&) = delete;
    BusLinux_t operator=(const BusLinux_t&) = delete;

    BusLinux_t(BusLinux_t&&) = default;
    BusLinux_t& operator=(BusLinux_t&&) = default;

    Msg_t Read() const noexcept override
    {
      return Msg_t{};
    }

    BusStatus_t Write(const Msg_t&) noexcept override
    {
      return BusStatus_t::Inactive;
    }
};

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

    void attach_timer_to_task(const BaseTask_t&, Millis_t) override
    {
    }
    void start_task(const BaseTask_t&) override
    {
    }
    void suspend_task(const BaseTask_t&) override
    {
    }
    void resume_task(const BaseTask_t&) override
    {
    }
};

int main(void)
{
  using Node_t = Node<std::size_t, BusLinux_t, SpawnerLinux_t>;

  Node_t drone{SpawnerLinux_t{},BusLinux_t{}, [](){return 0;}};

  drone.async_start();

  return 0;
}
