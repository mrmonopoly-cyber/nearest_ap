#include "candidate.h"

using namespace nearest_ap;

Candidate_t::Candidate_t(const Id id) noexcept
: m_name(id)
{
}

bool Candidate_t::same(const Candidate_t& candidate) const noexcept
{
  return m_name == candidate.m_name;
}

bool Candidate_t::operator==(const Candidate_t& candidate) const noexcept
{
  return this->same(candidate);
}
