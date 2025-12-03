#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include <cstddef>
#include <cstdint>

#include "../base_task.h"
#include "../bus/bus.h"
#include "../../internal/internal.h"
extern "C"{
#include "../messages/out/leader_alive.pb.h"
#include "../messages/nanopb_src/pb_encode.h"
}

namespace nearest_ap {
  template< typename AddressType, std::size_t mex_payload_size = Bus<AddressType>::m_payload_max_size>
    class LeaderAliveTask : BaseTask_t
  {
    public:
      using Bus_t = Bus<AddressType, mex_payload_size>;
      using Msg_t = typename Bus_t::Msg;
      using VoteInfo_t = VoteInfo<AddressType>;
      using LocalPotentialInfo_t = LocalPotentialInfo<AddressType>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(
          const WaitFun_f& wait_f,
          Bus_t& bus,
          const LocalPotentialInfo_t& pot_info,
          const VoteInfo_t& vote_info) noexcept:
        m_bus(bus),
        m_wait_fun(wait_f),
        m_local_pot_info(pot_info),
        m_vote_info(vote_info)
        {
        }

      void run(void) noexcept override
      {
        Msg_t msg{};
        pb_ostream_t ostream{};

        ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
        while(1)
        {
          if (m_vote_info.is_leader())
          {
            near_ap_LeaderHeartbit heartbit =
            {
              .has_id = true,
              .id = static_cast<std::uint32_t>(m_vote_info.get_user()),
              .has_potential = true,
              .potential = m_local_pot_info.m_local_node.potential(),
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
          m_wait_fun(m_wait_time);
        }
      }

    private:
      Bus_t& m_bus;
      static constexpr Millis_t m_wait_time = 50;
      const WaitFun_f& m_wait_fun;
      const LocalPotentialInfo_t& m_local_pot_info;
      const VoteInfo_t& m_vote_info;
  };
};
