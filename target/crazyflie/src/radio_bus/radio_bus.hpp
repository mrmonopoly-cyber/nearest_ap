#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * INFO: the following description is not yet implemented. Right now there is no Quality Of Service
 * guarantees. The Below description if for future developmena
 *
 * The bus Reader implement a protocol to send messages reliably between all the drones in
 * the network using P2P radio broadcast.
 *
 * The messages are guaranteed to arrive in order to the destination. A message will not be sent to
 * another drone until the ack from the previous message is not arrived.
 *
 * Since the messages are all shared in broadcast the protocol will implement:
 *
 * - A DIFS wait time
 * - A congestion windows (like TCP)
 * - A SIFS wait time before sending
 * - A circular buffer for sending and receive (like TCP)
 *
 * Each sent message expect an ack and if an ack is not received, the respective message will be
 * sent again
 *
 *
 * The method Read will extract a new message from one of the drone in the network and works on polling
 *
 *
 */

#include <nearest_ap/scheduler/bus/bus.hpp>
#include <optional>

class RadioBus : public nearest_ap::Bus_t
{
  public:
    RadioBus();

    std::optional<Msg_t> Read() noexcept;
    nearest_ap::BusStatus_t Write(const Msg_t&) noexcept;

  private:
};
