#include "nearest_ap.h"

using namespace nearest_ap;


template<typename BusMex>
Node<BusMex>::Node(const SendMex send_f, const RecvMex recv_f) noexcept
: m_send_f(send_f), m_recv_f(send_f)
{
}
