#include "condvar.hpp"

#include "exceptions_calsses.hpp"
#include "mutex.hpp"

//#include <errno.h>
//#include <exception>
//#include <assert.h>

namespace advcpp
{
ConditionVar::ConditionVar()
: m_threadCount(0)
{
    m_conVar = PTHREAD_COND_INITIALIZER;
}

ConditionVar::~ConditionVar()
{
    int status = pthread_cond_destroy(&m_conVar);
    if (status){
        assert(status != EINVAL);
        assert(!"EBUSY - ConditionVar DTOR failed tried to close while close");
    }
}

void ConditionVar::Wait(Mutex& a_mutex)
{
    m_threadCount.Inc();
    int status = pthread_cond_wait(&m_conVar, &a_mutex.m_mutex);
    switch (status){
        case 0      :   
            return;
        case EINVAL :
            throw EinvalWait();
        case EPERM  :
            throw MtxDontBelong();
        default     :
            assert(!"wait failed");
    }
    m_threadCount.Dec();
}

void ConditionVar::Signal()
{
    int status = pthread_cond_signal(&m_conVar);
    if (!status){
        return;
    }
    assert(EINVAL != status);
    assert(!"Signal failed");
}

void ConditionVar::SignalAll() 
{
    int status = pthread_cond_broadcast(&m_conVar); 
    if (!status){
        return;
    } 
    assert(EINVAL != status);
    assert(!"Signal All Failed failed");  
}

int ConditionVar::GetCounter() const
{
    return m_threadCount.Get();
}

}//advcpp