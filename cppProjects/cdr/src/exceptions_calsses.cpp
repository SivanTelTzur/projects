#include "exceptions_calsses.hpp"


namespace advcpp
{

const char* MutexAlreadyLocked::what() const throw()
{
    return "the mutex already locked\n";
}

const char* MtxDontBelong::what() const throw()
{
    return "unable to unlock Mutex don't belong";
}

const char* MtxDestroyWhileLocked::what() const throw()
{
    return "attemp to destroy the object while locked";
}

const char* ConvDestroyWhileLocked::what()
{
    return "attemp to destroy the object while locked";
}

const char* EinvalWait::what()
{
    return "CondVar wait invalid var or different mutex ";
}

/**********Semaphore******************/
const char* SemMaxValue::what()
{
    return  "Value sent exceeds SEM_VALUE_MAX ";
}

const char* SemOverFlowReleases::what()
{
    return "The maximum allowable value for a semaphore would be exceeded ";
}

const char* SemSignalIntAcquire::what()
{
    return "call was interrupt by signal ";
}

const char* ThreadNotJoinableException::what() const throw()
{
    return "Thread is not joinable";
}

const char* ThreadDeadlockException::what() const throw()
{
    return "Thread is stuck in deadlock";
}

const char* ThreadLimitException::what() const throw()
{
    return "reached max number of open threads";
}

} // namespace advcpp
