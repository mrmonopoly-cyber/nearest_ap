#include "logger.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <sys/types.h>
#include <thread>
#include <chrono>

enum class Color : uint
{
  White = 37,
  BrightBlack = 90,
  BrightRed,
  BrightGreen,
  BrightYellow,
  BrightBlue,
  BrightMagenta,
  BrightCyan,
  BrightWhite,
};


static void _write_in(Color textColor,const char str[])
{
  std::cout << "\033[" << static_cast<uint>(textColor) << "m"
    << str << "\033[" << static_cast<uint>(Color::White) << "m";
}

void LinuxLogger::_print_log(LinuxLogger* self) noexcept
{
  while(true)
  {
    if (!self->m_log_queue.empty())
    {
      self->m_lock.lock();
      LogInstace& log = self->m_log_queue.front();
      auto in_time_t = std::chrono::system_clock::to_time_t(log.current_time);
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
              log.current_time.time_since_epoch()) % 1000;

      std::cout << "[";

      switch (log.level)
      {
        case nearest_ap::logger::Level::Debug:
          _write_in(Color::BrightCyan, "DEBUG");
          break;
        case nearest_ap::logger::Level::Info:
          _write_in(Color::BrightWhite, "INFO");
          break;
        case nearest_ap::logger::Level::Warning:
          _write_in(Color::BrightYellow, "WARNING");
          break;
        case nearest_ap::logger::Level::Error:
          _write_in(Color::BrightRed, "ERROR");
          break;
      }
      std::cout
        << "]"
        << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count() << " "
        << log.file << ":" << log.line << " " 
        << "\"" << log.str << "\""
        << std::endl;
      self->m_log_queue.pop();
      self->m_lock.unlock();
    }
  }
}

void LinuxLogger::log_full(const char* file, const int line,
    const nearest_ap::logger::Level level, std::string_view str) noexcept
{
  m_lock.lock();
  m_log_queue.emplace(file, line, std::chrono::system_clock::now(), level, str);
  m_lock.unlock();
}

LinuxLogger::LinuxLogger() noexcept
{
  std::thread th{LinuxLogger::_print_log, this};
  th.detach();

}
