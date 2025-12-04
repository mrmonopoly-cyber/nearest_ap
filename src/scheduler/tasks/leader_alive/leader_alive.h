#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include <cstdint>

#include "../base_task.h"
#include "../../bus/bus.h"
#include "../../../internal/internal.h"
#include "../../messages/out/leader_alive.pb.h"
#include "../../messages/nanopb_src/pb_encode.h"

namespace nearest_ap {
  template< typename AddressType, typename BusType >
    class LeaderAliveTask : BaseTask_t
  {
    public:
      using Msg_t = typename BusType::Bus::Msg_t;
      using Internal_t = Internal<AddressType>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(
          BusType& bus,
          const Internal_t& internal) :
        m_bus(bus),
        m_internal(internal)
        {
        }

      void run(void) noexcept override
      {
        Msg_t msg{};
        pb_ostream_t ostream{};

        ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
        while(1)
        {
          if (m_internal.is_leader())
          {
            near_ap_LeaderHeartbit heartbit =
            {
              .has_id = true,
              .id = static_cast<std::uint32_t>(m_internal.user_id()),
              .has_potential = true,
              .potential = m_internal.user_potential(),
            };

            if (pb_encode(&ostream, near_ap_LeaderHeartbit_fields, &heartbit))
            {
              BusStatus_t error = m_bus.Write(msg);
              if (error != BusStatus_t::Ok)
              {
                //TODO: manage failures in sending
              }
            }//TODO: manage failures in serialization
          }
        }
      }
    private:
      BusType& m_bus;
      const Internal_t& m_internal;
  };
};
