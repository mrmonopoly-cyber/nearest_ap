#pragma once

#include <cstddef>

namespace nearest_ap {

  template<typename AddressType>
  struct LocalPotentialInfo
  {
    class PotentialNode
    {
      public:
        using Potential = std::size_t;
        using Candidate_t = AddressType;

        PotentialNode() = delete;
        PotentialNode(const Candidate_t& user) :
          m_user(user),
          m_potential(0)
      {}

        bool is_stronger(Potential potential) const noexcept
        {
          return m_potential < potential;
        }

        void update(const Potential pot) noexcept
        {
          m_potential = pot;
        }
        const Candidate_t& name() const noexcept
        {
          return m_user;
        }

      private:
        const Candidate_t& m_user;
        Potential m_potential;
    };

    std::size_t tollerance = 0;
    PotentialNode m_local_node;
    PotentialNode m_leader;
  };
};
