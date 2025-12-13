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

  class Logger
  {
    public:
      virtual void log_full(
          const char* file,
          const int line,
          const Level,
          std::string_view str) noexcept =0;
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
