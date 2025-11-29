#include "tasks.h"

Task::Task(void* const data, const TFun fun) noexcept
  : m_data(data), m_fun(fun)
{
}

Task::Task(void* const  data, const TFun&& fun) noexcept
  : m_data(data), m_fun(fun)
{
}

Task::TaskError Task::run()
{
  return m_fun(m_data);
}
