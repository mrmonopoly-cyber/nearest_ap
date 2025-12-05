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
      using AddressType_t = std::uint32_t;
      using Round_t = VoteInfo_t::Round_t;
      using Potential_t = std::uint32_t;
      using ComputePot_f = std::function<int()>;

      Internal_t() = delete;
      Internal_t(const ComputePot_f&& compute_pot) noexcept;
      Internal_t(const ComputePot_f&& compute_pot, const AddressType_t current_user) noexcept;
      Internal_t(const ComputePot_f&& compute_pot, const AddressType_t&& current_user) noexcept;

      void check_and_set_leader(const AddressType_t& new_leader, const Potential_t pot) noexcept;

      AddressType_t user_id() const noexcept;

      void compute_user_potential() noexcept;
      Potential_t user_potential() const noexcept;
      Potential_t leader_potential() const noexcept;

      Round_t round() const noexcept;
      void update_round(Round_t round) noexcept;

      bool is_leader() const noexcept;
      bool is_leader(AddressType_t& user) const noexcept;

      void support() noexcept;
      void new_election() noexcept;

    private:
      struct UserData_t
      {
        UserData_t() noexcept :
          m_user_index(0),
          m_potential(0)
          {}

        UserData_t(std::size_t user_index) noexcept:
          m_user_index(user_index),
          m_potential(0)
          {}

        UserData_t(std::size_t user_index, Potential_t potential) noexcept :
          m_user_index(user_index),
          m_potential(potential)
          {}

        std::size_t m_user_index=0;
        Potential_t m_potential=0;
      };

      std::vector<AddressType_t> m_users;
      const ComputePot_f m_compute_local_potential;
      UserData_t m_current_user;
      UserData_t m_leader;

      VoteInfo_t m_vote_info;
  };
};
