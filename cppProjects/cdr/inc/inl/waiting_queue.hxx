#ifndef B1D8FD8A_59AB_46DC_ACE7_EE335162BC95_H__
#define B1D8FD8A_59AB_46DC_ACE7_EE335162BC95_H__

#include "waiting_queue.hpp"
#include "exceptions_calsses.hpp"

#include <exception>
#include <assert.h>

namespace advcpp
{

template<class Value>
MtWaitingQueue<Value>::MtWaitingQueue(size_t a_maxNOfElemets)
: m_queue()
, m_fullCondVar()
, m_emptyCondVar()
, m_queueActionLock()
, m_capacity(a_maxNOfElemets)
{ }

template<class Value>
MtWaitingQueue<Value>::~MtWaitingQueue()
{}


template<class Value>
void MtWaitingQueue<Value>::Enqueue(const Value& a_newVal)
{
    mutexLockAndCheck();
    while (isFullNoGuard()){
        m_fullCondVar.Wait(m_queueActionLock);
    }
    try{
        m_queue.push_back(a_newVal);
    }
    catch (const std::bad_alloc & a_ex){
        m_queueActionLock.Unlock();
        //assert(!"failed allocated");
        throw;
    }
    mutexUnLockAndCheck();
    m_emptyCondVar.Signal();
}


template<class Value>
Value MtWaitingQueue<Value>::Dequeue()
{
    mutexLockAndCheck();
    while (isEmptyNoGuard()) {
       m_emptyCondVar.Wait(m_queueActionLock);
    }
    Value val;
    try{
        val = m_queue.front();
        m_queue.pop_front();
    } catch(...){
        mutexUnLockAndCheck();
        throw;
    }
    mutexUnLockAndCheck();
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