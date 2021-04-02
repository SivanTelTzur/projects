#ifndef K2446669_5E02_448C_886F_8CD16D76D8A3_H__
#define K2446669_5E02_448C_886F_8CD16D76D8A3_H__

#include "mutex.hpp"
#include "condvar.hpp"
#include "noncopyable.hpp"

//#include <cstddef> //size_t
#include <deque>

namespace advcpp
{
template<class Value>
class MtWaitingQueue : private NonCopyable{
public:
    //static const size_t MIN_SIZE_QUEUE = 10;
    explicit MtWaitingQueue(size_t a_maxNOfElemets = 0);
    ~MtWaitingQueue();

    void Enqueue(const Value& a_newVal);
    Value Dequeue();

    bool IsEmpty() const;
    bool IsFull() const;

private:
    bool isEmptyNoGuard() const;
    bool isFullNoGuard() const;
    void mutexLockAndCheck();
    void mutexUnLockAndCheck();

private:
    std::deque<Value> m_queue;
    ConditionVar m_fullCondVar;
    ConditionVar m_emptyCondVar;
    mutable Mutex m_queueActionLock;
    size_t m_capacity;
};



} // namespace advcpp

#include "inl/waiting_queue.hxx"

#endif // K2446669_5E02_448C_886F_8CD16D76D8A3_H__
