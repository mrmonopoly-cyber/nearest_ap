#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>

class testOut
{
  public:
    testOut()noexcept;

    testOut(const std::filesystem::path path) noexcept;

    bool is_ready(void) const noexcept;

    void open(const std::filesystem::path path) noexcept;
    void start_test() noexcept;
    void end_test() noexcept;

  private:
    std::fstream m_out_file{};
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
