#pragma once

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

      Internal_t() noexcept;
      Internal_t(AddressType_t current_user) noexcept;

      void check_and_set_leader(const AddressType_t &new_leader, const Potential_t pot) noexcept;

      AddressType_t user_id() const noexcept;

      Potential_t user_potential() const noexcept;

      Round_t round() const noexcept;
      void update_round(Round_t round) noexcept;

      bool is_leader() const noexcept;
      bool is_leader(AddressType_t& user) const noexcept;

      void support() noexcept;

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
      UserData_t m_current_user;
      UserData_t m_leader;

      VoteInfo_t m_vote_info;
  };
};
