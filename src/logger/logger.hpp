#pragma once

#include <string_view>

namespace nearest_ap::logger
{
  enum class Level
  {
    Debug,
    Info,
    Warning,
    Error
  };

  class StaticLog;

  class Logger
  {
    private:
      friend StaticLog;
       virtual void log_full(
          const char* file,
          const int line,
          const Level,
          std::string_view str) noexcept =0;
      virtual void print_current_level() const noexcept=0;
      void set_level(Level) noexcept;
      Level level(void) const noexcept;

    private:
      Level m_level = Level::Debug;
  };

  class StaticLog
  {
    public:
      StaticLog() =delete;

      StaticLog(const StaticLog&) =delete;
      StaticLog& operator=(const StaticLog&) =delete;

      StaticLog(StaticLog&&) =delete;
      StaticLog& operator=(StaticLog&&) =delete;

      StaticLog(Logger*const);

      static void print_current_level() noexcept;
      static void set_level(Level) noexcept;
      static Level level(void) noexcept;

      static void log_full(
          const char* file,
          const int line,
          const Level level,
          const std::string_view str)noexcept;
  };
}

#define static_log(LEVEL, STR)\
  nearest_ap::logger::StaticLog::log_full( \
      __FILE__, \
      __LINE__, \
      LEVEL, \
      STR)
