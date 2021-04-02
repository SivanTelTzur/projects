#include "thread_impl.hpp"
#include "functor.hpp"
#include "logger.hpp" //GetLogger
#include "exceptions_calsses.hpp"
#include "cloning_proxy.hpp"


#include <errno.h> //errors return
#include <assert.h>
#include <unistd.h> //usleep
#include <iostream> //cerr

namespace advcpp
{
ThreadImplemant::ThreadImplemant(ObjThreadFunc& a_func)
: m_isJoined(false)
, m_isDetached(false)
{
    CloningProxy<ObjThreadFunc> proxy(a_func);
    proxy.Wait();
    const int TRIES = 3;
    unsigned int sleepTimeUs = 80;
    for (int i = 0; i < TRIES; ++i){
        int status = pthread_create(&m_thread, 0, ActivateThreadFunction, &proxy);

        switch (status){
            case 0      :
                proxy.Wait();
                return;
            case EINVAL :
            case EPERM  :
                assert(!"Thread created with 0 attr this can't be happening" );
            case EAGAIN :
                SleepMicroSeconds(sleepTimeUs);
                sleepTimeUs *= sleepTimeUs;
                continue;
        }
    }

    throw ThreadLimitException();
}

ThreadImplemant::~ThreadImplemant()
{
    if (!m_isJoined && !m_isDetached){
        assert(!"thread must be joined or detached before dtor");
        StartLogger().Message("thread must be joined or detached before dtor");
        std::terminate();
    }
}


void ThreadImplemant::Join()
{
    if (m_isJoined || m_isDetached){
        assert(!"Can't joined a non joined thread");
    }

    int status = pthread_join(m_thread, 0);
    switch (status){
        case 0      :
            m_isJoined = true;
            return;
        case ESRCH :
            assert("ESRCH can't happen");
        case EINVAL  :
            throw ThreadNotJoinableException();
        case EDEADLK :
            throw ThreadDeadlockException();
    }
}

void ThreadImplemant::Detach()
{
    if (m_isJoined || m_isDetached){
        assert(!"Can't joined a non joined thread");
    }

    int status = pthread_detach(m_thread);
    switch (status){
        case 0      :
            m_isDetached = true;
            return;
        case ESRCH :
            assert("ESRCH can't happen");
        case EINVAL  :
            throw ThreadNotJoinableException();
    }
}

void ThreadImplemant::Cancel()
{
    if (m_isJoined){
        assert(!"Can't joined a non joined thread");
    }

    int status = pthread_cancel(m_thread);
    switch (status){
        case 0      :
            if (m_isDetached){
                return;
            }
            void* exit;
            status = pthread_join(m_thread, &exit);
            if (0 == status && exit == PTHREAD_CANCELED){
                m_isJoined = true;
            }
            return;
        case ESRCH :
            assert(!"ESRCH can't happen");
        case EINVAL  :
            throw ThreadNotJoinableException();
        case EDEADLK  :
            throw ThreadDeadlockException();
    }
}

bool ThreadImplemant::operator==(const ThreadImplemant& a_anotherThread) const
{
    return pthread_equal(this->m_thread, a_anotherThread.m_thread);
}


bool ThreadImplemant::operator!=(const ThreadImplemant& a_anotherThread) const
{
    return !(*this == a_anotherThread);
}

void ThreadImplemant::Yield() throw()
{
    if(0 != pthread_yield()){
        assert(!"failed on yield");
    }
}

void ThreadImplemant::SleepMicroSeconds(unsigned int a_microSeconds)
{
    useconds_t givenTime = a_microSeconds;
    usleep(givenTime);
}

void* ThreadImplemant::ActivateThreadFunction(void* a_objFunc)
{
    assert(a_objFunc);
    CloningProxy<ObjThreadFunc>* objProxy =  static_cast<CloningProxy<ObjThreadFunc>*>(a_objFunc);

    ObjThreadFunc* objFunc = objProxy->Clone();
    objProxy->Signal();
    try{
        objFunc->operator()();
    }
    catch(const std::exception& a_ex){
        delete objFunc;
        StartLogger().Exception(a_ex);
    }
    //catch(...){
    //    g_logger.Message("Thread active function throw an undefined exception");
    //    throw;
    //}
    delete objFunc;
    return 0;
}


} // namespace advcpp
