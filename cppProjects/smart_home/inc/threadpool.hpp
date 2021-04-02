#ifndef C6AC7CEC_037F_44FF_818D_5D125FAA55E3_H__
#define C6AC7CEC_037F_44FF_818D_5D125FAA55E3_H__

#include "waitableQueue.hpp"
#include "waitableQueueException.hpp"
#include "thread_group.hpp" //sharedptr 
#include "threadpool_exceptions.hpp"





namespace advcpp
{

class ThreadPool : private NonCopyable
{
    typedef WaitableQueue<SharedPtr<ObjThreadFunc> > Container; 
public:
    ThreadPool(size_t a_nOfThreads = 4);
    ~ThreadPool();

    void SubmitTask(SharedPtr<ObjThreadFunc> a_newTask);

    void AddThreads(size_t a_nOfThreads);
    void RemoveThreads(size_t a_request);

    void Shutdown();
    void ShutdownImmediately();

private:
    Container m_task;
    AtomicValue<bool> m_shutdounOn;
    AtomicValue<size_t> m_nOfThreads;
    Mutex m_getTaskMtx;
    CondotionalVariable m_taskInQCondVar;
    SharedPtr<ObjThreadFunc> m_executeTasks;
    ThreadGroup m_threads;
};


} // namespace advcpp



#endif // C6AC7CEC_037F_44FF_818D_5D125FAA55E3_H__