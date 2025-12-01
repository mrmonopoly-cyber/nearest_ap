#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "../candidate/candidate.h"

namespace nearest_ap {
  template <uint32_t default_num_candidates = 5>
    class VoteInfo
    {
      public:
        static constexpr uint32_t m_default_num_candidates = default_num_candidates;
        VoteInfo() = delete;

        explicit VoteInfo(const Candidate& m_user) noexcept;

        VoteInfo(const uint32_t num_candidates) noexcept;
        void reset() noexcept;
        void support() noexcept;
        bool election_status() noexcept;
        bool is_leader() const noexcept;

      private:
        static constexpr uint32_t m_election_req_mex_size = 5; //INFO: for now. In the future it will be the sizeof the serialized Protobuf object
        uint32_t m_num_candidates = default_num_candidates;
        uint32_t m_consent;
        uint32_t m_election_id;
        Candidate& m_user;
    };
};
