#ifndef K2446669_5E02_448C_886F_8CD16D76D8A3_H__
#define K2446669_5E02_448C_886F_8CD16D76D8A3_H__

#include "mutex.hpp"
#include "condvar.hpp"
#include "noncopyable.hpp"
#include "atomic_value.hpp"

//#include <cstddef> //size_t
#include <deque>

namespace advcpp
{
template<class Value>
class MtWaitingQueue : private NonCopyable{
public:
    explicit MtWaitingQueue(size_t a_maxNOfElemets = 0);
    ~MtWaitingQueue();

    void Enqueue(const Value& a_newVal);
    Value Dequeue();

    bool IsEmpty() const;
    bool IsFull() const;

    void Close();
    bool IsActiveQueue();

private:
    bool isFullNoGuard() const;
    bool isEmptyNoGuard() const;
    void mutexLockAndCheck();
    void mutexUnLockAndCheck();

private:
    std::deque<Value> m_queue;
    ConditionVar m_fullCondVar;
    ConditionVar m_emptyCondVar;
    mutable Mutex m_queueActionLock;
    const size_t m_capacity;
    AtomicValue<bool> m_isClose;
};


} // namespace advcpp

#include "inl/waiting_queue.hxx"

#endif // K2446669_5E02_448C_886F_8CD16D76D8A3_H__
