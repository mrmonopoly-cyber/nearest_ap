#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string_view>

template<typename Duration>
class testOut
{
     static_assert(std::chrono::treat_as_floating_point_v<typename Duration::rep> ||
                  std::is_integral_v<typename Duration::rep>);
  public:
    testOut()noexcept:
      m_max_time_exceded(std::chrono::hours{9999999999})
    {
    }

    testOut(const Duration max_time_exceded)noexcept:
    m_max_time_exceded(max_time_exceded)
    {
    }

    testOut(
        const Duration max_time_exceded,
        const std::filesystem::path path) noexcept:
    m_max_time_exceded(max_time_exceded),
    m_out_file(path)
    {
    }

    bool max_time_excedeed(void) const noexcept
    {
      const auto time = std::chrono::high_resolution_clock::now();
      const Duration time_passed = std::chrono::duration_cast<Duration>(time - m_start);
      return time_passed > m_max_time_exceded;
    }

    bool is_ready(void) const noexcept
    {
      return m_out_file.is_open();
    }

    void open(const std::filesystem::path path) noexcept
    {
      m_out_file.open(path, std::fstream::ios_base::out | std::fstream::app);
    }

    void start_test() noexcept
    {
      m_start = std::chrono::high_resolution_clock::now();
    }

    void end_test() noexcept
    {
      if (max_time_excedeed())
      {
        std::string err = "out of bounds\n";
        m_out_file.write(err.c_str(), err.length());
      }
      else
      {
        const auto end_test = std::chrono::high_resolution_clock::now();
        std::ostringstream out{};
        out << std::chrono::duration<double>(end_test - m_start).count();
        const std::string str{out.str() + '\n'};
        m_out_file.write(str.c_str(), str.length());
      }

    }

  private:
    Duration m_max_time_exceded;
    std::fstream m_out_file{};
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
