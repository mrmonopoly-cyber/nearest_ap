#pragma once


#include <mutex>
#include <nearest_ap/logger/logger.hpp>
#include <queue>
#include <string>

class LinuxLogger : public nearest_ap::logger::Logger
{
  public:
    explicit LinuxLogger() noexcept;

    void log_full(
          const char* file,
          const int line,
          const nearest_ap::logger::Level level,
          std::string_view str
        ) noexcept override;

  private:
      struct LogInstace
      {
        const char*const file;
        const int line;
        const std::chrono::system_clock::time_point current_time;
        const nearest_ap::logger::Level level;
        const std::string str;

        LogInstace(
            const char*const file,
            const int line,
             const std::chrono::system_clock::time_point current_time,
            const nearest_ap::logger::Level level,
            const std::string_view str) noexcept:
          file(file), line(line), current_time(current_time), level(level), str(str)
        {
        }
      };

    void print_current_level() const noexcept override;

    static void _print_log(LinuxLogger*) noexcept;

  private:
    std::mutex m_lock;
    std::queue<LogInstace> m_log_queue;
};
