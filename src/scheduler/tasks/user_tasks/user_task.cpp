#include "user_task.hpp"
#include "../event_queue/event_queue.hpp"

using namespace nearest_ap;

UserTask_t::UserTask_t(EventWriter& queue) noexcept :
m_queue(queue)
{
}
