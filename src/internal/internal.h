#pragma once

#include "vote_info/vote_info.h"
#include "potential_info/potential_info.h"

namespace nearest_ap 
{
  template<typename AddressType>
  struct Internal
  {
    const AddressType m_current_user;
    VoteInfo<AddressType> m_vote_info;
    LocalPotentialInfo<AddressType> m_local_potential_info;
  };
};
