#pragma once

#include <cstddef>

namespace nearest_ap {
  template<typename AddressType>
    class VoteInfo
    {
      public:
        VoteInfo() = delete;
        explicit VoteInfo(const AddressType& user, AddressType& leader) noexcept :
          m_consent(0),
          m_round(0),
          m_user(user),
          m_leader(leader)
          {}

        void reset() noexcept
        {
          m_consent =0;
        }

        void support() noexcept
        {
          m_consent++;
        }

        bool election_winner() noexcept
        {
          return m_consent > (m_num_candidates/2);
        }

        bool is_leader() const noexcept
        {
          return m_user == m_leader;
        }

        const AddressType& get_user() const noexcept
        {
          return m_user;
        }

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        std::size_t m_round=0;
        const AddressType& m_user;
        AddressType& m_leader;
    };
};
