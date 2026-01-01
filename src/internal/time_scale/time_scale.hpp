#pragma once

namespace nearest_ap
{
  class TimeScale
  {
    public:
      using Unit = float;

      inline constexpr Unit get(void) const noexcept
      {
        return m_time_scale_factor;
      }

      inline void increment(void) noexcept
      {
        m_time_scale_factor++;
      }

      inline void reset(void) noexcept
      {
        m_time_scale_factor=1;
      }

      inline void decrement(void) noexcept
      {
        if (m_time_scale_factor>0)
        {
          m_time_scale_factor--;
        }
      }

      inline Unit operator++(void) noexcept
      {
        increment();
        return m_time_scale_factor;
      }

      inline Unit operator--(void) noexcept
      {
        decrement();
        return m_time_scale_factor;
      }

    private:
      Unit m_time_scale_factor=1;
  };
}

