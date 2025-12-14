#include "internal.hpp"
#include <cstdint>

using namespace nearest_ap;

using VirtualId_t = Internal_t::VirtualId_t;
using Potential_t = Internal_t::Potential_t;
using Round_t = Internal_t::Round_t;
using ComputePot_f = Internal_t::ComputePot_f;
using Topology = Internal_t::Topology;

//INFO: Constructor

Internal_t::Internal_t(
    Topology&& topology,
    const std::uint16_t current_user_index,
    const ComputePot_f&& compute_pot
    ) noexcept
: m_users(topology),
  m_current_user_index(current_user_index),
  m_user_potential(0),
  m_leader_potential(~0),
  m_received_heartbit(0),
  m_compute_local_potential(compute_pot),
  m_tollerance(0),
  m_vote_info(m_users.m_elements.size())
{
  compute_user_potential();
}

Internal_t::Internal_t(
    Topology&& topology,
    const std::uint16_t current_user_index,
    const ComputePot_f&& compute_pot,
    const Tollerance_t tollerance
    ) noexcept
: m_users(topology),
  m_current_user_index(current_user_index),
  m_user_potential(0),
  m_leader_potential(~0),
  m_received_heartbit(0),
  m_compute_local_potential(compute_pot),
  m_tollerance(tollerance),
  m_vote_info(m_users.m_elements.size())
{
  compute_user_potential();
}

//INFO: Obvserver

bool Internal_t::leader(void) const noexcept
{
  return m_users.m_leader_index == m_current_user_index;
}

bool Internal_t::election_sent(void) const noexcept
{
  return m_vote_info.election_sent();
}

bool Internal_t::strong_pot(void) const noexcept
{
  return user_pot() > m_leader_potential + m_tollerance;
}

bool Internal_t::voted(void) const noexcept
{
  return m_vote_info.voted();
}

Round_t Internal_t::round(void) const noexcept
{
  return m_vote_info.round();
}

Potential_t Internal_t::user_pot(void) const noexcept
{
  return m_user_potential;
}

Potential_t Internal_t::leader_pot(void) const noexcept
{
  return m_leader_potential;
}

VirtualId_t Internal_t::user_id(void) const noexcept
{
  return m_users.m_elements[m_current_user_index];
}

//INFO: Modifiers

void Internal_t::new_election(void) noexcept
{
  m_vote_info.start_new_election();
}

void Internal_t::recv_heartbit(
    const VirtualId_t leader_id,
    const Potential_t leader_pot,
    const Round_t leader_round) noexcept
{
  if (leader_round < round())
  {
    return;
  }

  const auto strong_leader_pot = leader_pot >= user_pot();
  if (m_users.leader() == leader_id || strong_leader_pot)
  {
    m_leader_potential = leader_pot;
    if (leader_round > round())
    {
      m_vote_info.update_round(leader_round);
      m_vote_info.end_election();
    }
    if (strong_leader_pot)
    {
      m_users.update_leader(leader_id);
    }
    m_received_heartbit++;
  }
}

bool Internal_t::support_check_wining(void) noexcept
{
  m_vote_info.support();
  const bool res = m_vote_info.won();

  if (res)
  {
    m_vote_info.end_election();
    m_users.m_leader_index = m_current_user_index;
    m_leader_potential = m_user_potential;
  }

  return res;
}

void Internal_t::vote_for(const Round_t round, const VirtualId_t user) noexcept
{
  m_vote_info.vote(round, leader());
  (void) user; //TODO: add an use case in future
}

void Internal_t::compute_user_potential(void) noexcept
{
  m_user_potential = m_compute_local_potential();
  if (leader())
  {
    m_leader_potential = m_user_potential;
  }
}

bool Internal_t::check_heartbit() noexcept
{
  if (m_received_heartbit)
  {
    m_received_heartbit--;
  }

  return m_received_heartbit;
}
