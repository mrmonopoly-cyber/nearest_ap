#pragma once

#include <cstddef>

namespace nearest_ap {
  template<typename AddressType>
    class VoteInfo
    {
      public:
        VoteInfo() :
          m_num_candidates(1),
          m_consent(0),
          m_round(0)
      {}

        void start_new_election() noexcept
        {
          m_consent =0;
          m_round++;
        }

        void support() noexcept
        {
          if (m_consent < m_num_candidates)
          {
            m_consent++;
          }
        }

        bool won() const noexcept
        {
          return m_consent > (m_num_candidates/2);
        }

      private:
        std::size_t m_num_candidates = 1;
        std::size_t m_consent=0;
        std::size_t m_round=0;
    };
};
