#pragma once

#include <cstdint>
#include <cstring>
#include <string_view>

#ifdef __linux__
#include <cstdio>
#endif // __linux__

namespace nearest_ap::logger
{
  enum class Level
  {
    Debug,
    Info,
    Warning,
    Error
  };

  template <uint32_t size>
#ifdef __linux__
    class UserLog 
    {
      public:
        UserLog() = default;

        UserLog(std::string_view str) noexcept
          :m_buffer(str)
        {
        }

        inline std::string_view raw() const noexcept
        {
          return m_buffer;
        }

        inline uint32_t original_size() const noexcept
        {
          return size;
        }

        inline void reset() noexcept
        {
          m_cursor = m_buffer;
          m_available_space = size;
          memset(m_buffer, 0, size);
        }

        inline void append_msg(const std::string_view obj) noexcept
        {
          if (obj.length() <m_available_space)
          {
            const std::uint32_t written = snprintf(m_cursor, m_available_space,
                "%.*s", static_cast<int>(obj.size()),obj.data());
            m_cursor += written;
            m_available_space -= written;
          }
        }

        inline void append_msg(std::int32_t obj) noexcept
        {
          const std::uint32_t written = snprintf(m_cursor, m_available_space, "%d", obj);
          m_cursor += written;
          m_available_space -= written;
        }

        inline void append_msg(std::uint32_t obj) noexcept
        {
          const std::uint32_t written = snprintf(m_cursor, m_available_space, "%d", obj);
          m_cursor += written;
          m_available_space -= written;
        }

      private:
        char m_buffer[size]{};
        char* m_cursor = m_buffer;
        uint32_t m_available_space = size;
    };
#else
    class UserLog 
    {
      public:
        UserLog() = default;

        inline std::string_view raw() const noexcept
        {
          return {};
        }

        inline uint32_t original_size() const noexcept
        {
          return size;
        }

        inline void reset() noexcept
        {
        }

        inline void append_msg(const std::string_view) noexcept
        {
        }

        inline void append_msg(std::int32_t) noexcept
        {
        }

        inline void append_msg(std::uint32_t) noexcept
        {
        }

        inline void append_msg(bool) noexcept
        {
        }
    };

#endif // __linux__


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

      template<uint32_t size>
        static void log_full(
            const char* file,
            const int line,
            const Level level,
            const UserLog<size>& str)noexcept
        {
          if (StaticLog::g_logger)
          {
            if (level >= StaticLog::g_logger->level())
            {
              return StaticLog::g_logger->log_full(file, line, level, str.raw());
            }
          }
        }


    private:
      static Logger* g_logger;
  };
}

#define static_log(LEVEL, LOG)\
  nearest_ap::logger::StaticLog::log_full( \
      __FILE__, \
      __LINE__, \
      LEVEL, \
      LOG)
