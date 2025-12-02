#pragma once

#include <cstddef>

namespace nearest_ap
{
  class Candidate_t
  {
    public:
      using Id = std::size_t;

      Candidate_t(const Id id) noexcept;
      
      bool same(const Candidate_t& candidate) const noexcept;
      bool operator==(const Candidate_t& candidate) const noexcept;

    private:
      const Id m_name = 0;
  };
};
