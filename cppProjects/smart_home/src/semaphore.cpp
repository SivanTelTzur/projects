#include "semaphore.hpp"
#include "exceptions_calsses.hpp"

#include <exception>

namespace advcpp{


Semaphore::Semaphore(unsigned int a_value){
    if (0 != sem_init(&m_sem, 0, a_value)){
        throw SemMaxValue();
    }
}

Semaphore::~Semaphore() {
    sem_destroy(&m_sem);
}

void Semaphore::Acquire() {
    if (0 != sem_wait(&m_sem)){
        throw SemSignalIntAcquire();
    }
}

void Semaphore::Release() {
    if (0 != sem_post(&m_sem)){
        throw SemOverFlowReleases();
    }
}

int Semaphore::Get() const 
{
    int semVal;
    sem_getvalue(&m_sem, &semVal);
    return semVal;
}
} // namespace advcpp
