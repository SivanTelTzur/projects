#ifndef FBEAB104_ECD6_4CDC_992F_6E1F13D3307B_H__
#define FBEAB104_ECD6_4CDC_992F_6E1F13D3307B_H__

#include <vector>

#include "noncopyable.hpp"
#include "thread.hpp"
#include "stdlib.h" //size_t
#include "semaphore.hpp"
#include "functor.hpp" // ObjThreadFunc
#include "sharedptr.hpp"

namespace advcpp
{
class Thread;

class ThreadGroup : private NonCopyable{

public:
    ThreadGroup(size_t a_nOfThreads, SharedPtr<ObjThreadFunc> a_runnable);
    ~ThreadGroup();

    void Detach(size_t a_request);
    void DetachAll();
    

    void Join(size_t a_request);
    void JoinAll();

    template <typename Functor>
    void JoinAll(Functor a_func);
    
    void Cancel(size_t a_request);
    void CancelAll();

    size_t Add(size_t a_threads); //returns number of threads added

private:
    void addThreads(size_t a_nOfThreads);
    void deleteMembersCtor();

private:
    std::vector<Thread*> m_vec;
    SharedPtr<ObjThreadFunc> m_threadsFunc;
    Semaphore m_sem;
};


} // namespace advcpp
#include "inl/thread_group.hxx"

#endif // FBEAB104_ECD6_4CDC_992F_6E1F13D3307B_H__
