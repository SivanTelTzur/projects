#ifndef K3D926E2_3AD7_403D_8190_081E8C7C7AE3_H__
#define K3D926E2_3AD7_403D_8190_081E8C7C7AE3_H__

#include "noncopyable.hpp"

#include <pthread.h>

namespace advcpp
{
class ObjThreadFunc;

class ThreadImplemant : private NonCopyable
{
public:
    explicit ThreadImplemant(ObjThreadFunc& a_func);
    ~ThreadImplemant();

    operator pthread_t&() {return m_thread;}

    void Join();
    void Detach();
    void Cancel();

    bool operator==(const ThreadImplemant& a_anotherThread) const;
    bool operator!=(const ThreadImplemant& a_anotherThread) const;

public:
    static void SleepMicroSeconds(unsigned int a_microSeconds);
    static void Yield() throw();

private:
    static void* ActivateThreadFunction(void* a_objFunc);
private:
    pthread_t m_thread;
    bool m_isJoined;
    bool m_isDetached;
};

} // namespace advcpp

#endif //K3D926E2_3AD7_403D_8190_081E8C7C7AE3_H__