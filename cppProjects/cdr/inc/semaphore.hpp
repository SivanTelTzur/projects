#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "noncopyable.hpp"

#include <semaphore.h>

namespace advcpp{

//class SemMaxValue;
//class SemOverFlowReleases;
//class SemSignalIntAcquire;

class Semaphore : NonCopyable{
public:
    Semaphore(unsigned int a_value);
    ~Semaphore();

    void Acquire();
    void Release();

private:
    sem_t m_sem;
};

} // namespace advcpp

#endif // __SEMAPHORE_H__
