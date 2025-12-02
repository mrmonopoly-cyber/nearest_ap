#pragma once

#include <vector>

#include "candidate/candidate.h"
#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"

namespace nearest_ap {
  template <std::size_t defualt_num_candidates, std::size_t tollerance>
    struct Internal{
      Internal() = delete;

      VoteInfo<defualt_num_candidates> m_vote_info;
      LocalPotentialInfo<tollerance> m_local_potential_info;
      std::vector<Candidate_t> m_users;
    };
};
