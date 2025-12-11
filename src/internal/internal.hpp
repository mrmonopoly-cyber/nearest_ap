#pragma once

//Copyright (c) 2025 Alberto Damo. All Rights Reserved.

#include <functional>
#include <vector>
#include <cstdint>

#include "vote_info/vote_info.hpp"

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

      Internal_t() = delete;
      Internal_t(const ComputePot_f&& compute_pot) noexcept;
      Internal_t(const ComputePot_f&& compute_pot, const Tollerance_t tollerance) noexcept;
      Internal_t(
          const ComputePot_f&& compute_pot,
          const Tollerance_t tollerance,
          const VirtualId_t&& current_user) noexcept;


      void check_and_set_leader(const VirtualId_t& new_leader, const Potential_t pot) noexcept;

      VirtualId_t user_id() const noexcept;

      void compute_user_potential() noexcept;
      bool user_pot_better_leader_pot() const noexcept;
      Potential_t user_potential() const noexcept;

      Round_t round() const noexcept;
      void update_round(Round_t round) noexcept;

      bool is_leader() const noexcept;
      bool is_leader(VirtualId_t& user) const noexcept;

      void support() noexcept;
      void new_election() noexcept;
      inline bool in_election() const noexcept
      {
        return m_vote_info.in_election();
      }


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

      std::vector<VirtualId_t> m_users;
      const ComputePot_f m_compute_local_potential;
      const Tollerance_t m_tollerance;
      UserData_t m_current_user;
      UserData_t m_leader;

      VoteInfo_t m_vote_info;
  };
};
