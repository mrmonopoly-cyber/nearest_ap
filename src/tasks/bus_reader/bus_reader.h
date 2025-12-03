#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * BusReaderTask: receive messages from the bus and update internal data
 *
 * Possible messages:
 *
 * 1- LeaderAlive (potential): 
 *            potential_info.update_leader_pot(potential);
 * 2- NewElection (round, id, potential):
 *            IF (round > local round) AND (potential > local potential) THEN
 *              vote.update_round(round);
 *              SEND ElectionVote(round, id)
 * 3- ElectionVote (round, id):
 *            IF round == current election round THEN
 *              vote.increment();
 */

#include "../base_task.h"

#include "../../internal/internal.h"
#include "../bus/bus.h"

namespace nearest_ap {
  template<typename AddressType, std::size_t mex_size = Bus<AddressType>::m_payload_max_size >
    class BusReaderTask : public BaseTask_t
  {
    public:
      using Bus_t = Bus<AddressType, mex_size>;
      using VoteInfo_t = VoteInfo<AddressType>;
      using LocalPotentialInfo_t = LocalPotentialInfo<AddressType>;

      explicit BusReaderTask() = delete;

      BusReaderTask(
          Bus_t& bus,
          VoteInfo_t& vote_info,
          LocalPotentialInfo_t& local_potential
          ) noexcept:
        m_bus(bus),
        m_vote_info(vote_info),
        m_local_potential_info(local_potential)
        {
        }

      TaskError_t run(void) noexcept override
      {
        return TaskError_t::Error;
      }

    private:
      Bus_t& m_bus;
      VoteInfo_t& m_vote_info;
      LocalPotentialInfo_t & m_local_potential_info;
  };
};
