#ifndef I1957848_6A69_4CD4_B984_D85FA4F602BF_H__
#define I1957848_6A69_4CD4_B984_D85FA4F602BF_H__

#include <pthread.h>

namespace advcpp {

class Mutex;

class ConditionVar {
public:
    ConditionVar();
    ~ConditionVar();

    void Wait(Mutex& a_mutex);
    void Signal();

private:
    pthread_cond_t m_conVar;
};

} // namespace advcpp

#endif // I1957848_6A69_4CD4_B984_D85FA4F602BF_H__
