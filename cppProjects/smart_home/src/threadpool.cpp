#include "threadpool.hpp"
#include "logger.hpp"
#include "mutex_guard.hpp"
#include "sleep_micro_sec.hpp"
#include "semaphore.hpp"
#include "sharedptr.hpp"


namespace advcpp
{

class PredThreadPoolAndQStatus{
public:
    PredThreadPoolAndQStatus(WaitableQueue<SharedPtr<ObjThreadFunc> >& a_tasks, AtomicValue<bool>& a_shutdounOn, AtomicValue<bool>& a_removeOn)
    : m_tasks(a_tasks)
    , m_shutdounOn(a_shutdounOn)
    , m_removeOn(a_removeOn)
    {}

    bool operator()() {return m_tasks.IsEmpty() && !m_shutdounOn.Get() && m_tasks.IsActive() && !m_removeOn.Get();}

private:
    WaitableQueue<SharedPtr<ObjThreadFunc> >& m_tasks;
    AtomicValue<bool>& m_shutdounOn;
    AtomicValue<bool>& m_removeOn;
};

class ExcecuteTask : public ObjThreadFunc
{
public:
    ExcecuteTask(WaitableQueue<SharedPtr<ObjThreadFunc> >& a_tasks, AtomicValue<bool>& a_shutDownOn, CondotionalVariable& a_taskInQCondVar, Mutex& a_getTaskMtx)
    : m_tasks(a_tasks)
    , m_shutDownOn(a_shutDownOn)
    , m_taskInQCondVar(a_taskInQCondVar)
    , m_getTaskMtx(a_getTaskMtx)
    , m_cancelOnFlag(new AtomicValue<bool>(false))
    , m_cancelSem(new Semaphore(0))
    , m_semStuck(new AtomicValue<size_t>(0))
    {
    }
    void operator()()
    {
        while(m_tasks.IsActive()){

            if (m_cancelOnFlag->Get()){
                (*m_semStuck)++;
                m_cancelSem->Acquire();
            }

            m_getTaskMtx.Lock();
            m_taskInQCondVar.CondVarWaitT(m_getTaskMtx, PredThreadPoolAndQStatus(m_tasks, m_shutDownOn, *m_cancelOnFlag));
            if (!m_tasks.IsActive() || (m_tasks.IsEmpty() && m_shutDownOn.Get()) ){
                m_getTaskMtx.Unlock();
                return;
            }
            if (m_cancelOnFlag->Get()){
                m_getTaskMtx.Unlock();
                continue;                
            }
            SharedPtr<ObjThreadFunc> task(0);
            m_tasks.Dequeue(task);
            m_getTaskMtx.Unlock();
            (*task)();
        }
    }

    ObjThreadFunc* Clone() const
    {
        ObjThreadFunc* copy = 0;
        try{
            copy = new ExcecuteTask(*this);
        }
        catch (std::bad_alloc & a_ex){
            StartLogger().Exception(a_ex);
            throw;
        }
        return copy;
    }

    void CancelThreads(const AtomicValue<size_t>& a_numOfExistedThreads){
        m_cancelOnFlag->Set(true);
 
        while (m_semStuck->Get() < a_numOfExistedThreads.Get()){
            m_taskInQCondVar.CondVarSignalAll();
            __asm volatile ("pause" ::: "memory");        
        }
        //std::cerr << "*****" << "here CancelThreads" << std::endl;
    }

    int GetNOfThreadsCancelstop() const {return m_semStuck->Get();}
    void SetCancelFlagOn(){m_cancelOnFlag->Set(true);}
    void SetCancelFlagOff(){m_cancelOnFlag->Set(false);}
    void ReleaseThreadsAfterCancel(size_t a_nOfThreadsLeft){
        for (size_t i = 0; i < a_nOfThreadsLeft; ++i){
            m_cancelSem->Release();
        }
        m_semStuck->Set(0);
    }

private:
    WaitableQueue<SharedPtr<ObjThreadFunc> >& m_tasks;
    AtomicValue<bool>& m_shutDownOn;  
    CondotionalVariable& m_taskInQCondVar;
    Mutex& m_getTaskMtx;
    SharedPtr<AtomicValue<bool> > m_cancelOnFlag;
    SharedPtr<Semaphore> m_cancelSem;
    SharedPtr<AtomicValue<size_t> > m_semStuck; 

};

ThreadPool::ThreadPool(size_t a_nOfThreads) 
: m_task()
, m_shutdounOn(false)
, m_nOfThreads(a_nOfThreads)
, m_getTaskMtx()
, m_taskInQCondVar(0)
, m_executeTasks(new ExcecuteTask(m_task, m_shutdounOn, m_taskInQCondVar, m_getTaskMtx))
, m_threads(a_nOfThreads, m_executeTasks)
{
    
}

ThreadPool::~ThreadPool() 
{
    Shutdown();
    m_threads.JoinAll();
}

void ThreadPool::SubmitTask(SharedPtr<ObjThreadFunc> a_newTask) 
{
    if (m_shutdounOn.Get() || !m_task.IsActive()){
        throw ThreadPoolClosedForNewTasks();
    }
    
    try{
        m_task.Enqueue(a_newTask);
        m_taskInQCondVar.CondVarSignal();
    }
    catch (const PthreadContWaitFailed& a_ex){
        StartLogger().Exception(a_ex);
    }
    catch (const QueueFinishException& a_ex){
        //will be handled in phase 2
        StartLogger().Exception(a_ex);
        throw;
    }

}

void ThreadPool::AddThreads(size_t a_nOfThreads) 
{
    m_getTaskMtx.Lock();
        size_t newNOfThreads =  m_nOfThreads.Get() + m_threads.Add(a_nOfThreads);
    m_getTaskMtx.Unlock();
    m_nOfThreads.Set(newNOfThreads);
}

void ThreadPool::RemoveThreads(size_t a_request) 
{
    a_request = std::min(a_request, m_nOfThreads.Get()); 

    ExcecuteTask* executeFunc = static_cast<ExcecuteTask*>(m_executeTasks.operator->());

    executeFunc->CancelThreads(m_nOfThreads);

    m_threads.Cancel(a_request);

    executeFunc->SetCancelFlagOff();
    m_nOfThreads.Set(m_nOfThreads.Get()- a_request); 
    executeFunc->ReleaseThreadsAfterCancel(m_nOfThreads);
}

void ThreadPool::Shutdown() 
{
    m_shutdounOn.Set(true);
    while(m_taskInQCondVar.GetCounter()){
        __asm volatile ("pause" ::: "memory");
        m_taskInQCondVar.CondVarSignalAll();
    }
    //m_taskInQCondVar.CondVarSignalAll();
    m_threads.JoinAll();
}

void ThreadPool::ShutdownImmediately() 
{
    m_task.Finish(0);
    while(m_taskInQCondVar.GetCounter()){
        __asm volatile ("pause" ::: "memory");
        m_taskInQCondVar.CondVarSignalAll();
    }
    m_threads.JoinAll();
}

    
} // namespace advcpp
