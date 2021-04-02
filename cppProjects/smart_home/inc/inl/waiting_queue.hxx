#ifndef B1D8FD8A_59AB_46DC_ACE7_EE335162BC95_H__
#define B1D8FD8A_59AB_46DC_ACE7_EE335162BC95_H__

#include "waiting_queue.hpp"
#include "exceptions_calsses.hpp"
#include "mutex_guard.hpp"

#include <exception>
#include <stdexcept>
#include <assert.h>
#include <iostream>

namespace advcpp
{

template <typename T>
class IsEmptyPred
{
public:
    explicit IsEmptyPred(std::deque<T>& a_deque, AtomicValue<bool>& a_isClose)
    : m_deque(a_deque)
    , m_isClose(a_isClose)
    {}

    bool operator()()
    {   
        return (m_deque.empty() && !m_isClose.Get());
    }

private:
    std::deque<T>& m_deque;
    AtomicValue<bool>& m_isClose;
};

template <typename T>
class IsFullPred
{
public:
    explicit IsFullPred(std::deque<T>& a_deque, size_t a_capacity, AtomicValue<bool>& a_isClose)
    : m_deque(a_deque)
    , m_capacity(a_capacity)
    , m_isClose(a_isClose)
    {}

    bool operator()()
    {return (m_capacity && m_deque.size() == m_capacity && !m_isClose.Get());}

private:
    std::deque<T>& m_deque;
    size_t m_capacity;
    AtomicValue<bool>& m_isClose;
};

template<class Value>
MtWaitingQueue<Value>::MtWaitingQueue(size_t a_maxNOfElemets)
: m_queue()
, m_fullCondVar()
, m_emptyCondVar()
, m_queueActionLock()
, m_capacity(a_maxNOfElemets)
, m_isClose(false)
{ }

template<class Value>
MtWaitingQueue<Value>::~MtWaitingQueue()
{}


template<class Value>
void MtWaitingQueue<Value>::Enqueue(const Value& a_newVal)
{
    if(!IsActiveQueue()){
        throw std::runtime_error("q is closed");
    }
    {
        MutexGuard guard(m_queueActionLock);
        m_fullCondVar.Wait(m_queueActionLock, IsFullPred<Value>(m_queue, m_capacity, m_isClose));
        if (m_isClose.Get()){
            throw  std::runtime_error("Q is closed enqueue");
        }
        try{
            m_queue.push_back(a_newVal);
        }
        catch (const std::bad_alloc & a_ex){
            assert(!"push-back can't send exception full queue was checked");
            throw;
        }

    } //MutexGuard unlock

    m_emptyCondVar.Signal();
}

      //  std::cerr << m_deque.size()  << !m_isClose.Get()<< std::endl;//

template<class Value>
Value MtWaitingQueue<Value>::Dequeue()
{

    if (!IsActiveQueue()){
        throw std::runtime_error("Q is closed");
    }
    Value val;
    {
        MutexGuard guard(m_queueActionLock);
        m_emptyCondVar.Wait(m_queueActionLock, IsEmptyPred<Value>(m_queue, m_isClose));
        if (m_isClose.Get()){
            throw std::runtime_error("Q is closed");
        }
        try{
            val = m_queue.front();
            m_queue.pop_front();
        } catch(...){
            assert(!"pop_front can't send exception empty queue was checked");
            throw;
        }
    } //MutexGuard unlock

    m_fullCondVar.Signal();
    return val;
}

template<class Value>
bool MtWaitingQueue<Value>::IsEmpty() const
{
    m_queueActionLock.Lock();
    bool result = isEmptyNoGuard();
    m_queueActionLock.Unlock();
    return result;
}

template<class Value>
bool MtWaitingQueue<Value>::IsFull() const
{
    m_queueActionLock.Lock();
    bool result = isFullNoGuard();
    m_queueActionLock.Unlock();
    return result;
}


template<class Value>
void MtWaitingQueue<Value>::Close()
{
    m_isClose.Set(true);

    while (m_fullCondVar.GetCounter() || m_emptyCondVar.GetCounter()){
        m_emptyCondVar.SignalAll();
        m_fullCondVar.SignalAll();
        __asm volatile ("pause" ::: "memory");
    }

}

template<class Value>
bool MtWaitingQueue<Value>::IsActiveQueue()
{
    return !m_isClose.Get();
}


/*******Private func ******/

template<class Value>
void MtWaitingQueue<Value>::mutexLockAndCheck()
{
    try{
        m_queueActionLock.Lock();
    }
    catch (const MutexAlreadyLocked& a_ex){
        assert(!a_ex.what());
    }
}

template<class Value>
void MtWaitingQueue<Value>::mutexUnLockAndCheck()
{
    try{
        m_queueActionLock.Unlock();
    }
    catch (const MtxDontBelong & a_mtx){
        assert(!a_mtx.what());
    }
}

template<class Value>
bool MtWaitingQueue<Value>::isEmptyNoGuard() const
{
    return m_queue.empty();
}

template<class Value>
bool MtWaitingQueue<Value>::isFullNoGuard() const
{
    return (m_capacity && m_queue.size() == m_capacity );
}

} // namespace advcpp

#endif // B1D8FD8A_59AB_46DC_ACE7_EE335162BC95_H__
