#include "vote_info.hpp"

using namespace nearest_ap;

using Round_t = VoteInfo_t::Round_t;

VoteInfo_t::VoteInfo_t():
  m_num_candidates(1),
  m_consent(0),
  m_round(0)
  {}


  void VoteInfo_t::start_new_election() noexcept
{
  m_consent =0;
  m_round++;
  m_in_election = true;
}

bool VoteInfo_t::in_election() const noexcept
{
  return m_in_election;
}

void VoteInfo_t::support() noexcept
{
  if (m_consent < m_num_candidates)
  {
    m_consent++;
  }
}

bool VoteInfo_t::check_winning() noexcept
{
  bool res =m_consent > (m_num_candidates/2);
  if (res)
  {
    m_in_election = false;
  }

  return res;
}

Round_t VoteInfo_t::round() const noexcept
{
  return m_round;
}

void VoteInfo_t::update_round(Round_t round) noexcept
{
  m_round = round;
}
