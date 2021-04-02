#ifndef FE04F514_4730_4051_9DA8_0F46F0279B29_H__
#define FE04F514_4730_4051_9DA8_0F46F0279B29_H__

#include "noncopyable.hpp"

#include <pthread.h>
#include <sys/types.h>

namespace advcpp{
class Mutex : private NonCopyable{
public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();
private:
    //friend class ConditionVar ;
    friend class CondotionalVariable;
    pthread_mutex_t* get_native_mutex() const;
private:
    mutable pthread_mutex_t m_mutex;
};

}//advcpp

#endif // FE04F514_4730_4051_9DA8_0F46F0279B29_H__

