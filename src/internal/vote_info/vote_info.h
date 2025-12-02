#pragma once

#include "../candidate/candidate.h"

namespace nearest_ap {
    class VoteInfo_t
    {
      public:
        VoteInfo_t() = delete;
        explicit VoteInfo_t(const Candidate_t& user, Candidate_t& leader) noexcept :
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

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        std::size_t m_round=0;
        const Candidate_t& m_user;
        Candidate_t& m_leader;
    };
};
