#pragma once

#include "../candidate/candidate.h"

namespace nearest_ap {
  template <std::size_t default_num_candidates = 5>
    class VoteInfo
    {
      public:
        static constexpr std::size_t m_default_num_candidates = default_num_candidates;
        VoteInfo() = delete;

        explicit VoteInfo(const Candidate_t& m_user) noexcept;

        VoteInfo(const std::size_t num_candidates) noexcept;
        void reset() noexcept;
        void support() noexcept;
        bool election_status() noexcept;
        bool is_leader() const noexcept;

      private:
        static constexpr std::size_t m_election_req_mex_size = 5; //INFO: for now. In the future it will be the sizeof the serialized Protobuf object
        std::size_t m_num_candidates = default_num_candidates;
        std::size_t m_consent;
        std::size_t m_election_id;
        Candidate_t& m_user;
        Candidate_t& m_leader;
    };
};
