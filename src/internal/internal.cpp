#include "internal.hpp"
#include <cstdint>

using namespace nearest_ap;

using VirtualId_t = Internal_t::VirtualId_t;
using Potential_t = Internal_t::Potential_t;
using Round_t = Internal_t::Round_t;
using ComputePot_f = Internal_t::ComputePot_f;
using Topology = Internal_t::Topology;

Internal_t::Internal_t(
    Topology&& topology,
    const std::uint16_t current_user_index,
    const ComputePot_f&& compute_pot
    ) noexcept
: m_users(topology),
  m_current_user_index(current_user_index),
  m_user_potential(0),
  m_leader_potential(0),
  m_compute_local_potential(compute_pot),
  m_tollerance(0),
  m_vote_info()
{
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
  m_leader_potential(0),
  m_compute_local_potential(compute_pot),
  m_tollerance(tollerance),
  m_vote_info()
{
}

void Internal_t::check_and_set_leader(const VirtualId_t &new_leader, const Potential_t pot) noexcept
{
  if ((pot > m_leader_potential) ||
      (pot == m_leader_potential && new_leader < m_users.m_leader_index))
  {
    if(m_users.update_leader(new_leader))
    {
      m_leader_potential = pot;
    }
  }
}

VirtualId_t Internal_t::user_id() const noexcept
{
  return m_users.m_elements[m_current_user_index];
}

void Internal_t::compute_user_potential() noexcept
{
  m_user_potential = m_compute_local_potential();
  if (is_leader())
  {
    m_leader_potential = m_user_potential;
  }
}

Potential_t Internal_t::user_potential() const noexcept
{
  return m_user_potential;
}

bool Internal_t::user_pot_better_leader_pot() const noexcept
{
  return m_user_potential > m_leader_potential + m_tollerance;
}

bool Internal_t::is_leader() const noexcept
{
  return m_users.m_elements[m_current_user_index] == m_users.leader();
}

Round_t Internal_t::round() const noexcept
{
  return m_vote_info.round();
}

void Internal_t::update_round(Round_t round) noexcept
{
  m_vote_info.update_round(round);
}

void Internal_t::support() noexcept
{
  m_vote_info.support();
  if(m_vote_info.check_winning())
  {
    m_users.m_leader_index = m_current_user_index;
    m_leader_potential = m_user_potential;
  }
}

void Internal_t::new_election() noexcept
{
  m_vote_info.start_new_election();
}
