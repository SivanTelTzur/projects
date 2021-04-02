#ifndef F39EF1E6_C536_4720_805C_9B2064F4B18D_H__
#define F39EF1E6_C536_4720_805C_9B2064F4B18D_H__


#include "mutex.hpp"
#include "noncopyable.hpp"

namespace advcpp {

class MutexGuard : private NonCopyable {
public:
    MutexGuard(Mutex& a_mutex);
    ~MutexGuard();

    void UnlockMtx();

private:
    Mutex& m_mutex;
    bool m_wasLocked;
};

}//advcpp

#endif // F39EF1E6_C536_4720_805C_9B2064F4B18D_H__
