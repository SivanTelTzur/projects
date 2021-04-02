#ifndef A3A53DA1_CD45_4E98_AA90_C5AA0F118266_H__
#define A3A53DA1_CD45_4E98_AA90_C5AA0F118266_H__


#include "condvar.hpp"

#include <assert.h>
#include <exceptions_calsses.hpp>
#include <errno.h>
#include "mutex.hpp"

namespace advcpp
{

template<typename Predicate>
void ConditionVar::Wait(Mutex& a_mutex, Predicate a_pred) 
{
    m_threadCount.Inc();
    while (a_pred()){
        int status = pthread_cond_wait(&m_conVar, a_mutex.get_native_mutex());
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
    }    
    m_threadCount.Dec();
} 

} // namespace advcpp

#endif // A3A53DA1_CD45_4E98_AA90_C5AA0F118266_H__
