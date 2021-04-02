#ifndef __EXCEPTIONSCLASSES_H__
#define __EXCEPTIONSCLASSES_H__

#include <exception>
namespace advcpp{

/*******************Mutex*********************/
class MutexAlreadyLocked : public std::exception {
public:
    virtual const char* what() const throw();
};


class MtxDontBelong : public std::exception{
public:
    virtual const char* what() const throw();
};


class MtxDestroyWhileLocked : public std::exception{
public:
    virtual const char* what() const throw();
};

/*****************Condition_Var***************/


class ConvDestroyWhileLocked : public std::exception{
    virtual const char* what();
};


class EinvalWait : public std::exception{
    virtual const char* what();
};

/**********Semaphore******************/
class SemMaxValue : public std::exception{
    virtual const char* what();
};

class SemOverFlowReleases : public std::exception{
    virtual const char* what();
};

class SemSignalIntAcquire : public std::exception{
    virtual const char* what();
};


class ThreadNotJoinableException : public std::exception {
public:
    virtual const char* what() const throw();
};

class ThreadDeadlockException : public std::exception{
public:
    virtual const char* what() const throw();
};

class ThreadLimitException : public std::exception{
public:
    virtual const char* what() const throw();
};

} // namespace advcpp

#endif // __EXCEPTIONSCLASSES_H__
