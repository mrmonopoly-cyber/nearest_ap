#include "internal.hpp"

using namespace nearest_ap;

using VirtualId_t = Internal_t::VirtualId_t;
using Potential_t = Internal_t::Potential_t;
using Round_t = Internal_t::Round_t;
using ComputePot_f = Internal_t::ComputePot_f;

Internal_t::Internal_t(const ComputePot_f&& compute_pot) noexcept:
m_users({VirtualId_t()}),
  m_compute_local_potential(std::move(compute_pot)),
  m_tollerance(0),
  m_current_user(0,0),
  m_leader(0,0),
  m_vote_info()
{
}

Internal_t::Internal_t(
          const ComputePot_f&& compute_pot,
          const Tollerance_t tollerance) noexcept:
m_users({VirtualId_t()}),
  m_compute_local_potential(std::move(compute_pot)),
  m_tollerance(tollerance),
  m_current_user(0,0),
  m_leader(0,0),
  m_vote_info()
{
}

Internal_t::Internal_t(
          const ComputePot_f&& compute_pot,
          const Tollerance_t tollerance,
          const VirtualId_t&& current_user) noexcept:
m_users({current_user}),
  m_compute_local_potential(std::move(compute_pot)),
  m_tollerance(tollerance),
  m_current_user(0,0),
  m_leader(0,0),
  m_vote_info()
{
}


void Internal_t::check_and_set_leader(const VirtualId_t &new_leader, const Potential_t pot) noexcept
{
  if (pot > m_leader.m_potential)
  {
    for (uint32_t i=0; i<m_users.size(); i++)
    {
      if (m_users[i] == new_leader)
      {
        m_leader.m_user_index = i;
        m_leader.m_potential = pot;
      }
    }
  }
}

VirtualId_t Internal_t::user_id() const noexcept
{
  return m_users[m_current_user.m_user_index];
}

void Internal_t::compute_user_potential() noexcept
{
  m_current_user.m_potential = m_compute_local_potential();
}

Potential_t Internal_t::user_potential() const noexcept
{
  return m_current_user.m_potential;
}

bool Internal_t::user_pot_better_leader_pot() const noexcept
{
  return m_current_user.m_potential > m_leader.m_potential + m_tollerance;
}

bool Internal_t::is_leader() const noexcept
{
  return m_current_user.m_user_index == m_leader.m_user_index;
}

bool Internal_t::is_leader(VirtualId_t& user) const noexcept
{
  return m_users[m_leader.m_user_index] == user;
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
  return m_vote_info.support();
}

void Internal_t::new_election() noexcept
{
  m_vote_info.start_new_election();
}
