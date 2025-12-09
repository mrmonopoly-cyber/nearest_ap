#pragma once

#include <nearest_ap/scheduler/bus/bus.hpp>
#include <queue>

class RadioBus : public nearest_ap::Bus_t
{
  public:
    RadioBus();

    Msg_t Read() noexcept;
    nearest_ap::BusStatus_t Write(const Msg_t&) noexcept;

  private:
};
