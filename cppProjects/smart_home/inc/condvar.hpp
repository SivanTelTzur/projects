#ifndef I1957848_6A69_4CD4_B984_D85FA4F602BF_H__
#define I1957848_6A69_4CD4_B984_D85FA4F602BF_H__

#include <pthread.h>

#include "atomic_counter.hpp"

namespace advcpp {

class Mutex;

class ConditionVar {
public:
    ConditionVar();
    ~ConditionVar();

    void Wait(Mutex& a_mutex);
    template <typename Predicate>
    void Wait(Mutex& a_mutex, Predicate a_pred);

    void Signal();
    void SignalAll();

    int GetCounter() const;

private:
    pthread_cond_t m_conVar;
    AtomicCounter<int> m_threadCount;

};


} // namespace advcpp

#include "inl/condvar.hxx"

#endif // I1957848_6A69_4CD4_B984_D85FA4F602BF_H__

