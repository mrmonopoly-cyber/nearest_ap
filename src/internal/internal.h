#pragma once

#include "candidate/candidate.h"
#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"

namespace nearest_ap 
{
  struct Internal_t
  {
    Candidate_t m_current_user;
    VoteInfo_t m_vote_info;
    LocalPotentialInfo_t m_local_potential_info;
  };
};
