#include <cassert>

#include "thread_group.hpp"
#include "functor.hpp" // funcObject
#include "atomic_value.hpp"
namespace advcpp
{

struct JoinMembers{
    void operator()(Thread* a_currThread)
    {
        try{
            a_currThread->Join();
            delete a_currThread;
        }
        catch(const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("Join failed - JoinMembers");
        }
    }   
};

struct DetachMembers{
    void operator()(Thread* a_currThread)
    {
        try{
            a_currThread->Detach();
            delete a_currThread;
        }
        catch(const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("Detach failed - DetachMembers");
        }
    }   
};

struct CancelMembers{
    void operator()(Thread* a_currThread)
    {
        try{
            a_currThread->Cancel();
            delete a_currThread;
        }
        catch(const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("Cancel failed - CancelMembers");
        }
    }   
};

struct DeleteThread{
    void operator()(Thread* a_currThread)
    {
        a_currThread->Join();
        delete a_currThread; 
    }   
};



class SuspendExcute : public ObjThreadFunc{
public:
    SuspendExcute(Semaphore& a_sem, SharedPtr<ObjThreadFunc> a_threadFunc, AtomicValue<bool>& a_toCancel)
    : m_threadFunc(a_threadFunc)
    , m_sem(a_sem)
    , m_toCancel(a_toCancel)
    {    
        
    }

    ~SuspendExcute()
    {
    }

    virtual void operator()()
    {
        m_sem.Acquire(); //thread been created
        while (m_sem.Get() != 0){
            __asm volatile ("pause" ::: "memory");
            if (m_toCancel.Get()){
                return;
            }
        }
        return (*m_threadFunc)();
    }

    virtual ObjThreadFunc* Clone() const
    {
        try{
            ObjThreadFunc* copy = new SuspendExcute(m_sem, m_threadFunc, m_toCancel);
            return copy;
        }
        catch (std::bad_alloc & a_ex){
            StartLogger().Exception(a_ex);
            std::terminate();
        }   
    }

private:
    SharedPtr<ObjThreadFunc> m_threadFunc;
    Semaphore& m_sem;
    AtomicValue<bool>& m_toCancel;
};


ThreadGroup::ThreadGroup(size_t a_nOfThreads,  SharedPtr<ObjThreadFunc> a_runnable) 
: m_vec()
, m_threadsFunc(a_runnable)
, m_sem(a_nOfThreads)

{   
    try{
        addThreads(a_nOfThreads);
    }
    catch (const std::exception& a_ex){
        deleteMembersCtor();
        m_vec.clear();
        throw;
    }
}

ThreadGroup::~ThreadGroup() 
{
    
    if (0 < m_vec.size()){
        assert(!"threads must be joined or detached before dtor");
        StartLogger().Message("threads must be joined or detached before dtor");
        std::terminate();
    }

}

void ThreadGroup::Detach(size_t a_request) 
{  
    a_request = std::min(a_request, m_vec.size()); 
    std::vector<Thread*>::reverse_iterator  begin = m_vec.rbegin();
    std::vector<Thread*>::reverse_iterator  end = begin;
    end += a_request;
    try{
        for_each(begin, end, DetachMembers());    
    }
    catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("DetachMembers failed ");
    }
    m_vec.erase(end.base(), m_vec.end());

}

void ThreadGroup::DetachAll() 
{
    Detach(m_vec.size());
}

void ThreadGroup::Join(size_t a_request) 
{
    a_request = std::min(a_request, m_vec.size()); 
    std::vector<Thread*>::reverse_iterator  begin = m_vec.rbegin();
    std::vector<Thread*>::reverse_iterator  end = begin;
    end += a_request;
    try{
        for_each(begin, end, JoinMembers()); 
    }
    catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("JoinMembers failed ");
    }
    m_vec.erase(end.base(), m_vec.end()); 
  
}

void ThreadGroup::JoinAll() 
{
    Join(m_vec.size());
}

void ThreadGroup::Cancel(size_t a_request) 
{
    a_request = std::min(a_request, m_vec.size()); 
    std::vector<Thread*>::reverse_iterator  begin = m_vec.rbegin();
    std::vector<Thread*>::reverse_iterator  end = begin;
    end += a_request;
    try{
        for_each(begin, end, CancelMembers());    
    }
    catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("CancelMembers failed ");
    }
    m_vec.erase(end.base(), m_vec.end());       
}

void ThreadGroup::CancelAll() 
{
    Cancel(m_vec.size());   
}

size_t ThreadGroup::Add(size_t a_threads) 
{
    m_vec.reserve(m_vec.size() + a_threads);
    size_t i = 0;
    try{
        for( ; i < a_threads; ++i){
            Thread* temp = new Thread(*m_threadsFunc);
            m_vec.push_back(temp);        
        }
    } catch (const std::runtime_error& a_ex){

    }
    return i;
}

void ThreadGroup::addThreads(size_t a_nOfThreads) 
{
    AtomicValue<bool> stop(false);
    SharedPtr<ObjThreadFunc> waitForAll(new SuspendExcute(m_sem, m_threadsFunc, stop));

    m_vec.reserve(m_vec.size() + a_nOfThreads);
    for (size_t i = 0; i < a_nOfThreads ; ++i){
        try{
            Thread* temp = new Thread(waitForAll);
            m_vec.push_back(temp);
        }
        catch (const std::exception& a_ex){
            stop.Set(true);
            throw;
        }
    }

}

void ThreadGroup::deleteMembersCtor() 
{
    try{
        for_each (m_vec.begin(), m_vec.end(), DeleteThread());    
    }
    catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("deleteMembersCtor failed ");
    }
}
    
} // namespace advcpp

