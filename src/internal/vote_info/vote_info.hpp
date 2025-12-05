#pragma once

#include <cstddef>

namespace nearest_ap {
    class VoteInfo_t
    {
      public:
        using Round_t = std::size_t;

        VoteInfo_t();

        void start_new_election() noexcept;

        void support() noexcept;

        bool won() const noexcept;

        Round_t round() const noexcept;
        void update_round(Round_t round) noexcept;

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        Round_t m_round=0;
    };
};
