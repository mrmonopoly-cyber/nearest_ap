#include "logger.hpp"
#include <string_view>

using namespace nearest_ap::logger;

Logger* g_logger = nullptr;


void Logger::set_level(Level level) noexcept
{
  m_level = level;
}

Level Logger::level() const noexcept
{
  return m_level;
}

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
    if (level >= g_logger->level())
    {
      return g_logger->log_full(file, line, level, str);
    }
  }
}

void StaticLog::set_level(Level l) noexcept
{
  if (g_logger)
  {
    g_logger->set_level(l);
    g_logger->print_current_level();
  }
}

Level StaticLog::level(void) noexcept
{
  if (g_logger)
  {
    return g_logger->level();
  }
  return Level::Debug;
}
