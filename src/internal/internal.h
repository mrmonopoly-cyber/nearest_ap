#pragma once


#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"

struct Internal{
  VoteInfo m_vote_info;
  LocalPotentialInfo m_local_potential_info;

  Internal() = delete;
};
