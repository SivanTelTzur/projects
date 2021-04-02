#ifndef WAITABLEQUEUE_HPP_
#define WAITABLEQUEUE_HPP_

#include <stddef.h>
#include <queue> 

#include "conditionalVariable.hpp"
#include "conditionalVariableException.hpp"
#include "mutex.hpp"
#include "noncopyable.hpp"
#include "atomic_value.hpp"

namespace advcpp{

template <class varT>
class WaitableQueue : private NonCopyable{
public:
    typedef std::queue<varT> Cont;

    WaitableQueue(size_t a_queueSize = 0);
    ~WaitableQueue();

    void Enqueue(varT& a_val);
    void Dequeue(varT& a_val);

    bool IsEmpty() const;
    bool IsFull() const;

    void Finish(size_t a_threads);
    bool IsActive() const;

private:
    bool isEmptyNoneBlocking() const;
    bool isFullNoneBlocking() const;

private:
    size_t m_capacity;
    Cont m_cont;
    mutable Mutex m_mutex;
    CondotionalVariable m_condVarEnqueue;
    CondotionalVariable m_condVarDequeue;
    bool m_haveSize;   
    AtomicValue<bool> m_finish;   
};

} //advcpp

#include "inl/waitableQueue.hxx"

#endif //WAITABLEQUEUE_HPP_
