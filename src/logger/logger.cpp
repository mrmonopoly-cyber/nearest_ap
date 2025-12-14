#include "logger.hpp"
#include <string_view>

using namespace nearest_ap::logger;


namespace nearest_ap::logger
{
  Logger* StaticLog::g_logger = nullptr;
};

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
  StaticLog::g_logger = logger;
}

void StaticLog::set_level(Level l) noexcept
{
  if (StaticLog::g_logger)
  {
   StaticLog::g_logger->set_level(l);
   StaticLog::g_logger->print_current_level();
  }
}

Level StaticLog::level(void) noexcept
{
  if (StaticLog::g_logger)
  {
    return StaticLog::g_logger->level();
  }
  return Level::Debug;
}


void StaticLog::log_full(
    const char* file,
    const int line,
    const Level level,
    const std::string_view str)noexcept
{
          if (StaticLog::g_logger)
          {
            if (level >= StaticLog::g_logger->level())
            {
              return StaticLog::g_logger->log_full(file, line, level, str);
            }
          }
}
