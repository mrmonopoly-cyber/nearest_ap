#pragma once

#include <cstdint>

namespace nearest_ap {
  template <uint32_t defualt_num_candidates>
    class VoteInfo
    {
      public:
        VoteInfo(const uint32_t num_candidates) noexcept;

      private:
        uint32_t m_num_candidates = defualt_num_candidates;
    };
};
