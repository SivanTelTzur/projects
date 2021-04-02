#include "conditionalVariable.hpp"

#include <assert.h>
#include <errno.h>

namespace advcpp{

CondotionalVariable::CondotionalVariable(const pthread_condattr_t *a_restrict)
: m_counter(0)
{
    if(pthread_cond_init(&m_cond, a_restrict) != 0){
        assert(errno != EBUSY);
        throw PthreadContInitFailed();
    }
}

CondotionalVariable::~CondotionalVariable(){
    if(pthread_cond_destroy(&m_cond) != 0){
        assert(!"destroy failed");
    }
}

void CondotionalVariable::CondVarWait(Mutex& a_mutex){
    if(pthread_cond_wait(&m_cond, a_mutex.get_native_mutex()) != 0){
        throw PthreadContWaitFailed();
    } 
}

size_t CondotionalVariable::GetCounter() const
{
    return m_counter.Get();
}
/*
void CondotionalVariable::CondVarWait(Mutex& a_mutex,PredicateInterface& a_pred){
    while (a_pred()){
        if(pthread_cond_wait(&m_cond, a_mutex.getMutex()) != 0){
        throw PthreadContWaitFailed();
        } 
    }
}*/

void CondotionalVariable::CondVarSignal() throw(){
    if(pthread_cond_signal(&m_cond) != 0){
        assert(errno != EINVAL);
    }
}

void CondotionalVariable::CondVarSignalAll() throw(){
    if(pthread_cond_broadcast(&m_cond) != 0){
        assert(errno != EINVAL);
    }
}



} // advcpp
