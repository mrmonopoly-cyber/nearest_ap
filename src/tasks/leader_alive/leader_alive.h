#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.


/*
 * LeaderAliveTask: every L time:
 *        IF vote.leader() THEN
 *          SEND LeaderAlive(vote.user(), potential_info.potential())
 */

#include "../base_task.h"
#include "../bus/bus.h"
#include "../../internal/internal.h"

namespace nearest_ap {
  template< typename AddressType, std::size_t mex_payload_size = Bus<AddressType>::m_payload_max_size>
    class LeaderAliveTask : BaseTask_t
  {
    public:
      using Bus_t = Bus<AddressType, mex_payload_size>;
      using VoteInfo_t = VoteInfo<AddressType>;

      explicit LeaderAliveTask() noexcept;

      LeaderAliveTask(Bus_t& bus, const VoteInfo_t& vote_info) noexcept:
        m_bus(bus),
        m_vote_info(vote_info)
        {
        }

      TaskError_t run(void) noexcept override
      {
        return TaskError_t::Error;
      }

    private:
      Bus_t& m_bus;
      const VoteInfo_t& m_vote_info;
  };
};
