#include "logger.hpp"
#include <string_view>

using namespace nearest_ap::logger;

Logger* g_logger = nullptr;


StaticLog::StaticLog(Logger*const logger)
{
  g_logger = logger;
}

void StaticLog::log_full(
    const char* file,
    const int line,
    const Level level,
    const std::string_view str) noexcept
{
  if (g_logger)
  {
    return g_logger->log_full(file, line, level, str);
  }
}
