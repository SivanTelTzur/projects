#ifndef __CONDITIONALVARIABLE_HPP_
#define __CONDITIONALVARIABLE_HPP_
#include <pthread.h>

#include "atomic_counter.hpp"
#include "mutex.hpp"
#include "conditionalVariableException.hpp"

class PredicateInterface;

namespace advcpp{

class CondotionalVariable {
public:
    CondotionalVariable(const pthread_condattr_t *a_restrict);
    ~CondotionalVariable();

    void CondVarWait(Mutex& a_mutex);

    template<class UnaryPredicate>
    void CondVarWaitT(Mutex& a_mutex,UnaryPredicate a_pred);

    void CondVarSignal() throw();
    void CondVarSignalAll() throw();

    size_t GetCounter() const;

private:
    pthread_cond_t m_cond;
    AtomicCounter<size_t> m_counter;
};

template<class UnaryPredicate>
void CondotionalVariable::CondVarWaitT(Mutex& a_mutex,UnaryPredicate a_pred)
{
    m_counter.Inc();
    while (a_pred()){
        if(pthread_cond_wait(&m_cond, a_mutex.get_native_mutex()) != 0){
        throw PthreadContWaitFailed();
        } 
    }
    m_counter.Dec();
}


} // experis

#endif //__CONDITIONALVARIABLE_HPP_