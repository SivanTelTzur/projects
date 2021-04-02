#ifndef  D2F2F5A_5417_410D_A41F_57B26530FCDB_H__
#define D2F2F5A_5417_410D_A41F_57B26530FCDB_H__

#include "noncopyable.hpp"
#include "functor.hpp"
//#include "headerfilethread.hpp"

//#include <pthread.h>
namespace advcpp
{

class ThreadImplemant;
class Thread : private NonCopyable{
public:
    explicit Thread(ObjThreadFunc& a_func);
    ~Thread();

    void Join();
    void Detach();

    void Cancel();
    bool operator==(const Thread& a_anotherThread) const;
    bool operator!=(const Thread& a_anotherThread) const;

public:
    static void SleepMicroSeconds(unsigned int a_microSeconds);
    static void Yield() throw();

//private:
//    static void* ActivateThreadFunction(void* a_objFunc);
private:
    ThreadImplemant* m_thread;
    //ThreadType m_pthread;
    //bool m_isJoined;
    //bool m_isDetached;
};

}//advcpp

#endif // D2F2F5A_5417_410D_A41F_57B26530FCDB_H__
