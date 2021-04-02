#include "mutex_guard.hpp"

namespace advcpp {

MutexGuard::MutexGuard(Mutex& a_mutex)
: m_mutex(a_mutex)
, m_wasLocked(true)
{
    m_mutex.Lock();
}

MutexGuard::~MutexGuard()
{   
    if (m_wasLocked) {
        m_mutex.Unlock();
    }
}

void MutexGuard::UnlockMtx()
{
    if (m_wasLocked) {
        m_mutex.Unlock();
    }
}

}//advcpp
