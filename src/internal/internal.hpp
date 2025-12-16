#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

#include <atomic>
#include <functional>
#include <cstdint>

#include <nearest_ap/internal/vote_info/vote_info.hpp>

namespace nearest_ap 
{

  class Internal_t
  {
    public:
      using VirtualId_t = std::uint32_t;
      using Round_t = VoteInfo_t::Round_t;
      using Potential_t = std::uint32_t;
      using ComputePot_f = std::function<int()>;
      using Tollerance_t = std::uint32_t;

      struct Topology
      {
        public:
          VirtualId_t leader() const noexcept
          {
            return m_elements[m_leader_index];
          }

          bool update_leader(VirtualId_t leader) noexcept
          {
            for (uint32_t i=0; i<m_elements.size(); i++)
            {
              if (m_elements[i] == leader)
              {
                m_leader_index = i;
                return true;
              }
            }
            return false;
          }

        public:
          const std::vector<VirtualId_t> m_elements;
          std::uint32_t m_leader_index;
      };


      Internal_t() = delete;

      Internal_t(
          Topology&& topology,
          const std::uint16_t current_user_index,
          const ComputePot_f&& compute_pot
          ) noexcept;

      Internal_t(
          Topology&& topology,
          const std::uint16_t current_user_index,
          const ComputePot_f&& compute_pot,
          const Tollerance_t tollerance) noexcept;

      bool leader(void) const noexcept;
      bool election_sent(void) const noexcept;
      bool voted(void) const noexcept;
      Round_t round(void) const noexcept;
      Potential_t user_pot(void) const noexcept;
      Potential_t leader_pot(void) const noexcept;
      Potential_t better_candidate_pot(void) const noexcept;
      VirtualId_t user_id(void) const noexcept;
      VirtualId_t better_candidate(void) const noexcept;

      bool user_valid_for_election(void) noexcept;
      void new_election(void) noexcept;
      bool recv_heartbit(
          const VirtualId_t leader_id,
          const Potential_t leader_pot,
          const Round_t round) noexcept;
      void recv_heartbit_best_candidate(const VirtualId_t candidate_id, const Potential_t pot) noexcept;
      bool support_check_wining(void) noexcept;
      void abort_election(const Round_t round, const VirtualId_t leader, const Potential_t leader_pot) noexcept;
      void compute_user_potential(void) noexcept;
      void update_round(Round_t round) noexcept;
      void maybe_new_best_candidate(const VirtualId_t candidate, const Potential_t pot) noexcept;

    private:
      bool _consume_heartbit(std::atomic_uint32_t& who) noexcept;

    private:
      struct UserData_t
      {
        using UserIndex = std::uint32_t;

        UserData_t() noexcept :
          m_user_index(0),
          m_potential(0)
          {}

        UserData_t(UserIndex user_index) noexcept:
          m_user_index(user_index),
          m_potential(0)
          {}

        UserData_t(UserIndex user_index, Potential_t potential) noexcept :
          m_user_index(user_index),
          m_potential(potential)
          {}

        UserIndex m_user_index=0;
        Potential_t m_potential=0;
      };

      Topology m_users;
      const std::uint32_t m_current_user_index;
      Potential_t m_user_potential;
      Potential_t m_leader_potential;
      Potential_t m_best_candidate_pot;
      std::atomic_uint32_t m_received_heartbit_leader;
      std::atomic_uint32_t m_received_heartbit_best_candidate;
      const ComputePot_f m_compute_local_potential;
      const Tollerance_t m_tollerance;
      VirtualId_t m_best_candidate;

      VoteInfo_t m_vote_info;
  };
};
