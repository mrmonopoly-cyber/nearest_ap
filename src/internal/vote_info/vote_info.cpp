#include "vote_info.hpp"

#include <nearest_ap/logger/logger.hpp>

using namespace nearest_ap;

using Round_t = VoteInfo_t::Round_t;

//INFO: constructor

VoteInfo_t::VoteInfo_t(const uint32_t num_candidates) noexcept:
m_num_candidates(num_candidates)
{}

//INFO: observer

bool VoteInfo_t::election_sent(void) const noexcept
{
  return m_election_sent;
}

bool VoteInfo_t::voted(void) const noexcept
{
  return m_voted;
}

bool VoteInfo_t::won(void) const noexcept
{
  return m_consent > (m_num_candidates/2);
}

Round_t VoteInfo_t::round(void) const noexcept
{
  return m_round;
}

//INFO: modifiers

void VoteInfo_t::start_new_election(void) noexcept
{
  m_consent=1;
  m_election_sent = true;
  m_voted = true;
}

void VoteInfo_t::end_election(void) noexcept
{
  m_round++;
  m_election_sent = false;
  m_voted = false;
  m_consent =0;
}

void VoteInfo_t::renunce(void) noexcept
{
  m_election_sent = false;
  m_voted = false;
  m_consent =0;
}

void VoteInfo_t::support(void) noexcept
{
  m_consent++;
}

void VoteInfo_t::vote(const Round_t round, const bool leader) noexcept
{
  if (!m_voted)
  {
    if (!leader)
    {
      update_round(round);
    }
    m_voted = true;
  }
}

void VoteInfo_t::update_round(const Round_t round) noexcept
{
  m_round = round;
}
