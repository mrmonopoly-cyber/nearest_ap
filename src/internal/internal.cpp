#include "internal.hpp"
#include <cstdint>
#include <nearest_ap/logger/logger.hpp>
#include <utility>

using namespace nearest_ap;

using VirtualId_t = Internal_t::VirtualId_t;
using Potential_t = Internal_t::Potential_t;
using Round_t = Internal_t::Round_t;
using ComputePot_f = Internal_t::ComputePot_f;
using Topology = Internal_t::Topology;

//INFO: Constructor

Internal_t::Internal_t(
    Topology topology,
    const std::uint16_t current_user_index,
    const ComputePot_f compute_pot
    ) noexcept
: m_users(std::move(topology)),
  m_current_user_index(current_user_index),
  m_user_potential(0),
  m_leader_potential(0),
  m_best_candidate_pot(0),
  m_received_heartbit_leader(0),
  m_compute_local_potential(std::move(compute_pot)),
  m_tollerance(0),
  m_best_candidate(m_current_user_index),
  m_vote_info(m_users.m_num_elements)
{
  compute_user_potential();
}

Internal_t::Internal_t(
    Topology topology,
    const std::uint16_t current_user_index,
    const ComputePot_f compute_pot,
    const Tollerance_t tollerance
    ) noexcept
: m_users(std::move(topology)),
  m_current_user_index(current_user_index),
  m_user_potential(0),
  m_leader_potential(0),
  m_best_candidate_pot(0),
  m_received_heartbit_leader(0),
  m_compute_local_potential(std::move(compute_pot)),
  m_tollerance(tollerance),
  m_best_candidate(m_current_user_index),
  m_vote_info(m_users.m_num_elements)
{
  compute_user_potential();
}

//INFO: Obvserver

bool Internal_t::leader(void) const noexcept
{
  return 
    m_users.m_leader == m_current_user_index &&
    user_pot() >= better_candidate_pot()
    ;
}

bool Internal_t::election_sent(void) const noexcept
{
  return m_vote_info.election_sent();
}

bool Internal_t::voted(void) const noexcept
{
  return m_vote_info.voted();
}

bool Internal_t::is_best_candidate(void) const noexcept
{
  return m_best_candidate == m_current_user_index;
}

VirtualId_t Internal_t::better_candidate(void) const noexcept
{
  return m_best_candidate;
}

Potential_t Internal_t::better_candidate_pot(void) const noexcept
{
  return m_best_candidate_pot;
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
  return m_current_user_index;
}

//INFO: Modifiers

bool Internal_t::user_valid_for_election(void) noexcept
{
  const auto heartbit = _consume_heartbit(m_received_heartbit_leader);
  const auto heartbit_best_candidate = _consume_heartbit(m_received_heartbit_best_candidate);
  const auto user_is_best_candidate = m_current_user_index == m_best_candidate;
  const auto pot_user_better_leader = user_pot() > m_leader_potential + m_tollerance;
  const auto pot_user_better_candidate = user_pot() > m_best_candidate_pot + m_tollerance;

  if(!leader() && !heartbit)
  {
    logger::UserLog<128>log{};
    log.append_msg("node: ");
    log.append_msg(user_id());
    log.append_msg(" NO HEARTBIT FROM LEADER: ");
    log.append_msg(m_users.leader());

    if (!heartbit_best_candidate)
    {
      log.append_msg(" NO HEARTBIT FROM BEST CANDIDATE: ");
      log.append_msg(m_best_candidate);

      m_users.m_leader = m_users.leader();
      m_leader_potential = m_user_potential;
      m_best_candidate = m_current_user_index;
      m_best_candidate_pot = m_user_potential;
      m_potential_election_time_scale.reset();

      if(is_best_candidate())
      {
        m_users.update_leader(m_current_user_index);
        m_leader_potential = m_user_potential;
      }
    }
    else if(m_users.leader() == m_best_candidate)
    {
      if (m_received_heartbit_best_candidate> 2)
      {
        m_received_heartbit_best_candidate--;
      }
    }
    else
    {

      if (m_potential_election_time_scale.get()< 5)
      {
        ++m_potential_election_time_scale;
      }
      m_users.m_leader = m_best_candidate;
      m_leader_potential = m_best_candidate_pot;
    }

    static_log(logger::Level::Warning, log);

    return true;
  }

  if( 
      !leader() && 
      pot_user_better_leader && 
      (
       pot_user_better_candidate || user_is_best_candidate
      )
    )
  {
    if (!is_best_candidate() && m_potential_election_time_scale.get()< 5)
    {
      ++m_potential_election_time_scale;
    }
    else if(is_best_candidate())
    {
      m_potential_election_time_scale.reset();
    }

    return true;
  }

  return false;
}

void Internal_t::new_election(void) noexcept
{
  m_vote_info.start_new_election();
}

bool Internal_t::recv_heartbit(
    const VirtualId_t leader_id,
    const Potential_t leader_pot,
    const Round_t leader_round) noexcept
{
  if (leader_round < round())
  {
    return false;
  }

  if (round() < leader_round)
  {
    m_best_candidate = leader_id;
    m_best_candidate_pot = leader_pot;
    m_leader_potential = leader_pot;
    m_users.update_leader(leader_id);
    if (m_received_heartbit_leader < m_users.m_num_elements) {
      m_received_heartbit_leader++;
    }
    if (m_received_heartbit_best_candidate < m_users.m_num_elements*2) {
      m_received_heartbit_best_candidate+=2;
    }
    m_vote_info.update_round(leader_round);
    m_potential_election_time_scale.reset();
    return true;
  }

  if (
      (leader_pot >= user_pot() && leader_pot >= m_best_candidate_pot)
      ||
      leader_id == m_users.leader()
      ||
      (leader_id == m_best_candidate || leader_pot >= m_best_candidate_pot)
      )
  {
    m_best_candidate = leader_id;
    m_leader_potential = leader_pot;
    m_users.update_leader(leader_id);
    m_vote_info.renunce();
    if (m_received_heartbit_leader < m_users.m_num_elements) {
      m_received_heartbit_leader++;
    }
    if (m_received_heartbit_best_candidate < m_users.m_num_elements*2) {
      m_received_heartbit_best_candidate+=2;
    }
    return true;
  }

  return false;
}

void Internal_t::recv_heartbit_best_candidate(const VirtualId_t leader_id, const Potential_t pot) noexcept
{
  if (leader_id == m_best_candidate)
  {
    if (m_received_heartbit_best_candidate < m_users.m_num_elements*2)
    {
      m_received_heartbit_best_candidate+=2;
    }
    m_best_candidate_pot = pot;
  }
  else if(pot > m_best_candidate_pot)
  {
    m_received_heartbit_best_candidate=2;
    m_best_candidate = leader_id;
    m_best_candidate_pot = pot;
  }

}

bool Internal_t::support_check_wining(void) noexcept
{
  m_vote_info.support();
  const bool res = m_vote_info.won();

  if (res)
  {
    logger::UserLog<32>log{};
    log.append_msg("node: ");
    log.append_msg(m_current_user_index);
    log.append_msg(" won election");
    static_log(logger::Level::Warning, log);
    m_vote_info.end_election();
    m_users.m_leader = m_current_user_index;
    m_leader_potential = m_user_potential;
  }

  return res;
}

void Internal_t::abort_election(const Round_t round, const VirtualId_t leader_id, const Potential_t leader_pot) noexcept
{
  if (m_best_candidate_pot >= leader_pot)
  {
    m_vote_info.renunce();
    m_vote_info.update_round(round);
    m_vote_info.vote(round, leader());
    m_best_candidate = leader_id;
    m_best_candidate_pot = leader_pot;
  }
}

void Internal_t::compute_user_potential(void) noexcept
{
  m_user_potential = m_compute_local_potential();

  if (leader())
  {
    m_leader_potential = m_user_potential;
  }

  if(
      (user_pot() > m_best_candidate_pot + m_tollerance)
      ||
      (m_current_user_index == m_best_candidate)
    )
  {
    m_best_candidate_pot = m_user_potential;
    m_best_candidate = m_current_user_index;
  }
}

bool Internal_t::_consume_heartbit(std::atomic_uint32_t & who) noexcept
{
  if (who)
  {
    who--;
  }

  return who;
}

void Internal_t::update_round(Round_t round) noexcept
{
  m_vote_info.update_round(round);
}

void Internal_t::maybe_new_best_candidate(const VirtualId_t candidate, const Potential_t pot) noexcept
{
  if (pot > m_best_candidate_pot)
  {
    m_best_candidate = candidate;
    m_best_candidate_pot = pot;
  }
}
