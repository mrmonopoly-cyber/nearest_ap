#pragma once


#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"
#include <cstdint>

namespace nearest_ap {
  template <uint32_t defualt_num_candidates, uint32_t tollerance>
    struct Internal{
      VoteInfo<defualt_num_candidates> m_vote_info;
      LocalPotentialInfo<tollerance> m_local_potential_info;

      Internal() = delete;
    };
};
