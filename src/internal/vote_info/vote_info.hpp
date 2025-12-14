#pragma once

#include <cstddef>
#include <cstdint>

namespace nearest_ap {
    class VoteInfo_t
    {
      public:
        using Round_t = std::uint32_t;

        VoteInfo_t(void) noexcept = default;
        VoteInfo_t(const uint32_t num_candidates) noexcept;

        bool election_sent(void) const noexcept;
        bool voted() const noexcept;
        bool won(void) const noexcept;
        Round_t round(void) const noexcept;

        void start_new_election(void) noexcept;
        void end_election(void) noexcept;
        void renunce(void) noexcept;
        void support(void) noexcept;
        void vote(const Round_t, const bool leader) noexcept;
        void update_round(const Round_t round) noexcept;

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        Round_t m_round=0;
        bool m_election_sent = false;
        bool m_voted = false;
    };
};
