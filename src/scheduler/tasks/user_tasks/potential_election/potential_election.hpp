#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * PotentialElectionTask: every K time:
 *          update local potential = compute_potential();
 *          IF leader potential + tollerance < local potential AND vote.no_new_election() THEN:
 *              local round = vote.start_new_election();
 *              SEND NewElection(round, local id, local potential);
 */

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/user_tasks/user_task.hpp>
#include <nearest_ap/scheduler/tasks/event_queue/event_queue.hpp>

#include <project_deps.h>

namespace nearest_ap {
  template< typename BusType >
    class PotentialElectionTask : public UserTask_t
  {
    public:
      using Msg_t = typename BusType::Bus::Msg_t;

      explicit PotentialElectionTask() = delete;

      PotentialElectionTask(
          EventWriter& pipe,
          BusType& bus,
          Internal_t& internal) noexcept :
        UserTask_t(pipe),
        m_bus(bus),
        m_internal(internal)
        {
        }

      void run(void)override
      {
        m_internal.compute_user_potential();
        if (m_internal.user_potential() > m_internal.leader_potential()) //TODO: add tollerance
        {
          Msg_t msg{};
          pb_ostream_t ostream{};
          near_ap_NewElection new_election{
            .has_round = true,
            .round = m_internal.round(),
            .has_id = true,
            .id = m_internal.user_id(),
            .has_potential = true,
            .potential = m_internal.user_potential(),
          };

          ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());

          if (pb_encode(&ostream, near_ap_NewElection_fields, &new_election))
          {
            BusStatus_t error = m_bus.Write(msg);
            if (error != BusStatus_t::Ok)
            {
              //TODO: manage failures in sending
            }
          }//TODO: manage failures in serialization

          m_internal.new_election();
        }
      }

    private:
      BusType& m_bus;
      Internal_t& m_internal;
  };
};
