#include "waitableQueue.hpp"
#include "mutex_guard.hpp"
#include "waitableQueueException.hpp"

#include <unistd.h>

namespace advcpp{

template <typename T>
class PredicateEnq{
public:
    PredicateEnq(bool  a_haveSize,size_t a_capacity, std::queue<T>& a_Cont, AtomicValue<bool>& a_finish)
    :m_haveSize(a_haveSize)
    ,m_capacity(a_capacity)
    ,m_cont(a_Cont) 
    ,m_finish(a_finish)
    { }

    bool operator()() {return m_haveSize && m_capacity == m_cont.size() && m_finish == false;}
private:
    bool  m_haveSize;
    size_t m_capacity;
    std::queue<T>& m_cont;
    AtomicValue<bool>& m_finish;  
};

template <typename T>
class PredicateDeq{
public:
    PredicateDeq(std::queue<T>& a_Cont, AtomicValue<bool>& a_finish)
    :m_cont(a_Cont) 
    ,m_finish(a_finish)
    { }

    bool operator()() {return m_cont.empty() && m_finish == false ;}
    
private:
    std::queue<T>& m_cont;
    AtomicValue<bool>& m_finish;   
};

template <class varT>
WaitableQueue<varT>::WaitableQueue(size_t a_queueSize)
:  m_capacity(a_queueSize)
,  m_cont()
,  m_mutex()
,  m_condVarEnqueue(0)
,  m_condVarDequeue(0)
,  m_haveSize(a_queueSize ? true : false)
,  m_finish(false)
{
}

template <class varT>
WaitableQueue<varT>::~WaitableQueue(){
    // nothing to do
}

template <class varT>
void WaitableQueue<varT>::Enqueue(varT& a_val)
{
    {
        MutexGuard holder(m_mutex);
        try{
            m_condVarEnqueue.CondVarWaitT(m_mutex, PredicateEnq<varT>(m_haveSize, m_capacity, m_cont, m_finish));
        }catch(const PthreadContWaitFailed& a_err){
            throw;
        }
        if(m_finish == true){
            throw QueueFinishException();
        }
        try{
            m_cont.push(a_val);
        }
        catch(...){
            throw;
        }
    } //for mutexholder scope
    m_condVarDequeue.CondVarSignal();
}

template <class varT>
void WaitableQueue<varT>::Dequeue(varT& a_val){

    {
        MutexGuard holder(m_mutex);
        try{
            m_condVarDequeue.CondVarWaitT(m_mutex, PredicateDeq<varT>(m_cont, m_finish));
            }catch(const PthreadContWaitFailed& a_err){
                throw;
        }
        if(m_finish == true){
            throw QueueFinishException();
        }
        try{
            a_val = m_cont.front();
        }
        catch(...){
            throw;
        }
        m_cont.pop();
    } // mutex holder
    m_condVarEnqueue.CondVarSignal();
    return ;
}

template <class varT>
void WaitableQueue<varT>::Finish(size_t a_threads)
{
    m_finish.Set(true);
    for(size_t index = 0 ; index < m_condVarDequeue.GetCounter(); ++index){
        m_condVarDequeue.CondVarSignal();
    }
    for(size_t index = 0 ; index < m_condVarEnqueue.GetCounter(); ++index){
        m_condVarEnqueue.CondVarSignal();
    }
    while(m_condVarEnqueue.GetCounter() || m_condVarDequeue.GetCounter()){
        __asm volatile ("pause" ::: "memory");
        m_condVarDequeue.CondVarSignalAll();
        m_condVarEnqueue.CondVarSignalAll();
    }
}

template <class varT>
bool WaitableQueue<varT>::IsEmpty() const
{
    try{
         m_mutex.Lock();
    }
    catch(const std::exception& a_ex){
        throw;
    }
    bool empty = isEmptyNoneBlocking();
    try{
        m_mutex.Unlock();
    }
    catch(const std::exception& a_ex){
        throw;
    } 
    return empty;
}

template <class varT>
bool WaitableQueue<varT>::IsFull() const
{
   try{
         m_mutex.Lock();
    }
    catch(const std::exception& a_ex){
        throw;
    }
    bool empty = isFullNoneBlocking();
    try{
        m_mutex.Unlock();
    }
    catch(const std::exception& a_ex){
        throw;
    } 
    return empty;
}

template<class varT>
bool WaitableQueue<varT>::IsActive() const
{
    return !(m_finish.Get());    
}


template <class varT>
bool WaitableQueue<varT>::isEmptyNoneBlocking() const
{
    return m_cont.empty();
}

template <class varT>
bool WaitableQueue<varT>::isFullNoneBlocking() const
{
    /*if(m_capacity){
        return m_cont.size() == m_capacity;
    }
    return false;*/
    return m_capacity ? m_cont.size() == m_capacity : false;
}

} // advcpp






/*
template <class varT>
void WaitableQueue<varT>::Enqueue(varT& a_val){
    { // scope of mutexholder
        try{
            m_mutex.Lock();
        }
        catch(MutexAlreadyLokced()){
            throw;
        }
        while(m_haveSize && isFullNoneBlocking() ){
            try{
                m_condVarEnqueue.CondVarWait(&m_mutex);
            }
            catch(PthreadContWaitFailed()){
                throw;
            }
        }
        try{
            m_cont.push(a_val);
        }
        catch(...){
            throw;
        }
    }
    try{
        m_mutex.Unlock();
    }
    catch(const MutexAlreadyUnlokced& a_err){
        throw;
    } 
    m_condVarDequeue.CondVarSignal();
}
*/