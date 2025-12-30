#include "test_output.hpp"
#include <chrono>
#include <filesystem>

testOut::testOut() noexcept
{
}

testOut::testOut(const std::filesystem::path path) noexcept:
m_out_file(path)
{
}

bool testOut::is_ready(void) const noexcept
{
  return m_out_file.is_open();
}

void testOut::open(const std::filesystem::path path) noexcept
{
  m_out_file.open(path, std::fstream::ios_base::out |  std::fstream::app);
}

void testOut::start_test(void) noexcept
{
  m_start = std::chrono::high_resolution_clock::now();
}

void testOut::end_test(void) noexcept
{
  const auto end_test = std::chrono::high_resolution_clock::now();
  std::ostringstream out{};
  out << std::chrono::duration<double>(end_test - m_start).count();
  const std::string str{out.str() + '\n'};

  m_out_file.write(str.c_str(), str.length());
}
