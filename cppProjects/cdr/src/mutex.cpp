#include "mutex.hpp"
#include "exceptions_calsses.hpp"

#include <exception>
#include <assert.h>
#include <errno.h>

namespace advcpp{

Mutex::Mutex(){
    m_mutex = PTHREAD_MUTEX_INITIALIZER;
}

Mutex::~Mutex(){
    if ( 0 != pthread_mutex_destroy(&m_mutex)){
        assert(errno != EINVAL);
    }
}

void Mutex::Lock() {
    int status = pthread_mutex_lock(&m_mutex);
    if (!status){
        return;
    }
    if (status == EDEADLK){
        throw MutexAlreadyLocked();
    }
    assert(!"pthread_mutex_lock initialized with NULL attrs can never return error on lock");
}

void Mutex::Unlock(){
    if (0 != pthread_mutex_unlock(&m_mutex)){
        throw MtxDontBelong(); //EPERM
    }
}
}//advcpp


//class DestroyMtxFailed : public std::exception{
//    virtual const char* what() {return "Destroy Mutex Failed";}
//};
//class MutexAlreadyLocked : public std::exception{
//    virtual const char* what() {return "the mutex already locked";}
//};
//
//
//class MtxDontBelong : public std::exception{
//    virtual const char* what() {return "unable to unlock Mutex don't belong";}
//};
//
//
//class MtxDestroyWhileLocked : public std::exception{
//    virtual const char* what() {return "attemp to destroy the object while locked";}
//};