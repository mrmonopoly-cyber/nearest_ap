#include "spawner.h"
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>

using namespace nearest_ap;

using namespace std::chrono;
using namespace std::this_thread;

static void _start_task(std::function<void(void)> f)
{
  std::thread th{f};
  th.detach();
}

void SpawnerLinux_t::start_task(BaseTask_t* t) noexcept 
{
  std::cout
    << "staring task: " << t->id()
    << std::endl;

  _start_task([t]{while(true)t->run();});
}

void SpawnerLinux_t::start_task(BaseTask_t* t, Millis_t f) noexcept
{
  std::cout
    << "staring task: " << t->id()
    << " with freq: " << f
    << std::endl;

  _start_task([t,f]
        {
          while(true)
          {
            t->run();
            sleep_for(milliseconds(f));
          }
        }
      );
}
