#pragma once


#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"

template <uint32_t defualt_num_candidates>
struct Internal{
  VoteInfo<defualt_num_candidates> m_vote_info;
  LocalPotentialInfo m_local_potential_info;

  Internal() = delete;
};
