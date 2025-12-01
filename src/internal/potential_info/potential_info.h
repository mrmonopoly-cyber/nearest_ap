#pragma once

#include "../candidate/candidate.h"

#include <cstdint>
namespace nearest_ap {

  template <uint32_t tollerance= 0, typename Potential = uint32_t>
  struct LocalPotentialInfo
  {
      class PotentialNode
      {
        public:
          static constexpr uint32_t m_tollerance = tollerance;

          PotentialNode() = delete;
          PotentialNode(const Candidate& user);

          bool is_stronger(Potential potential) const noexcept;
          void update(const Potential pot) noexcept;
          const Candidate& name() const noexcept;

        private:
          const Candidate& m_user;
          Potential m_potential;
      };

      static constexpr uint32_t m_tollerance = tollerance;
      PotentialNode m_local_node;
      PotentialNode m_leader;
  };
};
