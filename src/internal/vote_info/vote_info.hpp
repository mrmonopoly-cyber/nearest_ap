#pragma once

#include <cstddef>
#include <cstdint>

namespace nearest_ap {
    class VoteInfo_t
    {
      public:
        using Round_t = std::uint32_t;

        VoteInfo_t();

        void start_new_election() noexcept;
        bool in_election() const noexcept;

        void support() noexcept;

        bool check_winning() noexcept;

        Round_t round() const noexcept;
        void update_round(Round_t round) noexcept;

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        Round_t m_round=0;
        bool m_in_election = false;
    };
};
