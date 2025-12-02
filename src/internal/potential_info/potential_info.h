#pragma once

#include <cstddef>

#include "../candidate/candidate.h"

namespace nearest_ap {

  template <
    std::size_t tollerance = 0,
    typename Potential = std::size_t >
  struct LocalPotentialInfo
  {
      class PotentialNode
      {
        public:
          static constexpr std::size_t m_tollerance = tollerance;

          PotentialNode() = delete;
          PotentialNode(const Candidate_t& user);

          bool is_stronger(Potential potential) const noexcept;
          void update(const Potential pot) noexcept;
          const Candidate_t& name() const noexcept;

        private:
          const Candidate_t& m_user;
          Potential m_potential;
      };

      static constexpr std::size_t m_tollerance = tollerance;
      PotentialNode m_local_node;
      PotentialNode m_leader;
  };
};
