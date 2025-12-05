#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

/*
 * BusReaderTask: receive messages from the bus and update internal data
 *
 * Possible messages:
 *
 * 1- NewElection (round, id, potential):
 *            IF (round > local round) AND (potential > local potential) THEN
 *              vote.update_round(round);
 *              SEND ElectionVote(round, id)
 * 2- ElectionVote (round, id):
 *            IF round == current election round THEN
 *              vote.increment();
 * 3- LeaderAlive (id, potential): 
 *              IF potential > local leader potential THEN
 *              vote.update_leader(id);
 *              potential_info.update_leader_pot(potential);
 */

#include <project_deps.h>

#include <nearest_ap/internal/internal.hpp>
#include <nearest_ap/scheduler/bus/bus.hpp>
#include <nearest_ap/scheduler/tasks/user_tasks/user_task.hpp>

namespace nearest_ap {
  template< typename BusType >
    class BusReaderTask : public UserTask_t
  {
    public:
      using Msg_t = typename BusType::Msg_t;

      explicit BusReaderTask() = delete;

      BusReaderTask(
          EventWriter& m_pipe,
          BusType& bus,
          Internal_t& internal) noexcept:
        UserTask_t(m_pipe),
        m_bus(bus),
        m_internal(internal)
        {
        }

      void run(void) noexcept override
      {
        Msg_t msg_raw{m_bus.Read()};
        _near_ap_MessageIndex msg_index{};
        pb_istream_t stream;

        stream = pb_istream_from_buffer(msg_raw.m_payload.data(), msg_raw.m_payload.size());
        pb_decode(&stream, near_ap_MessageIndex_fields, &msg_index);
        stream = pb_istream_from_buffer(
            reinterpret_cast<const pb_byte_t *>(msg_index.data),
            sizeof(msg_index.data)
            );

        switch (msg_index.msg_type)
        {
          case near_ap_MessageType_NewElection:
            {
              _near_ap_NewElection new_election{};
              pb_decode(&stream, near_ap_NewElection_fields, &new_election);
              if( new_election.round > m_internal.round() )
              {
                Msg_t msg{};
                pb_ostream_t ostream{};
                _near_ap_VoteRespone election_respone{
                  .has_round = true,
                    .round = new_election.round,
                    .has_id = true,
                    .id = m_internal.user_id(),
                };

                ostream = pb_ostream_from_buffer(msg.m_payload.data(), msg.m_payload.size());
                m_internal.update_round(new_election.round);
                if (pb_encode(&ostream, near_ap_VoteRespone_fields, &election_respone))
                {
                  BusStatus_t error = m_bus.Write(msg);
                  if (error != BusStatus_t::Ok)
                  {
                    //TODO: manage failures in sending
                  }
                }//TODO: manage failures in serialization

              }
            }
            break;
          case near_ap_MessageType_VoteRespone:
            {
              _near_ap_VoteRespone vote_response{};
              pb_decode(&stream, near_ap_VoteRespone_fields, &vote_response);
              if (m_internal.round() == vote_response.round)
              {
                m_internal.support();
              }
            }
            break;
          case near_ap_MessageType_LeaderHeartbit:
            {
              _near_ap_LeaderHeartbit leader_heartbit{};
              pb_decode(&stream, near_ap_LeaderHeartbit_fields, &leader_heartbit);
              m_internal.check_and_set_leader(leader_heartbit.id, leader_heartbit.potential);
            }
            break;
        }
      }

    private:
      BusType& m_bus;
      Internal_t& m_internal;
  };
};
