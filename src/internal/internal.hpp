#pragma once

#include <vector>
#include <cstdint>

#include "vote_info/vote_info.hpp"

namespace nearest_ap 
{
  template<typename AddressType>
  class Internal
  {
    public:
      using VoteInfo_t = VoteInfo<AddressType>;
      using Potential_t = std::uint32_t;

      Internal() noexcept:
        m_users({AddressType{}}),
        m_current_user(0,0),
        m_leader(0,0),
        m_vote_info()
        {}

      Internal(AddressType current_user) noexcept:
        m_users({AddressType{current_user}}),
        m_current_user(0,0),
        m_leader(0,0),
        m_vote_info()
        {}


      const AddressType& user_id() const noexcept
      {
        return m_users[m_current_user.m_user_index];
      }

      Potential_t user_potential() const noexcept
      {
        return m_current_user.m_potential;
      }

      bool is_leader() const noexcept
      {
        return m_current_user.m_user_index == m_leader.m_user_index;
      }

    private:
      struct UserData
      {
        UserData() noexcept :
          m_user_index(0),
          m_potential(0)
          {}

        UserData(std::size_t user_index) noexcept:
          m_user_index(user_index)
          {}

        UserData(std::size_t user_index, Potential_t potential) noexcept :
          m_user_index(user_index),
          m_potential(potential)
          {}

        std::size_t m_user_index=0;
        Potential_t m_potential=0;
      };

      std::vector<AddressType> m_users;
      UserData m_current_user;
      UserData m_leader;

      VoteInfo_t m_vote_info;
  };
};
