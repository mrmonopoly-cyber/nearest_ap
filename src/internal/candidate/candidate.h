#pragma once

#include <cstdint>

namespace nearest_ap
{
  class Candidate
  {
    public:
      using Id = uint32_t;

      Candidate(const Id id) noexcept;
      
      bool same(const Candidate& candidate);
      bool operator==(const Candidate& candidate) noexcept;

    private:
      const Id m_name = 0;
  };
};
