#ifndef L32FE72B_FEFD_4357_971D_C6C9620A2ABC_H__
#define L32FE72B_FEFD_4357_971D_C6C9620A2ABC_H__

#include "waiting_queue.hpp"
#include "cdr_struct.hpp"

namespace advcpp
{
class CdrHandler : private NonCopyable
{
    static std::string DIR_NAME;
    static const std::string IP;
    static const size_t PORT = 1200; 
public:
    CdrHandler();
    ~CdrHandler();

    void Run();

private:
    void startNotifierThread();
    void startReaderThread();
    void startCdrSenderNetTheard();
    void SleepMicroSeconds(unsigned int a_microSeconds);

private:
    MtWaitingQueue<Cdr> m_cdrs;
    MtWaitingQueue<std::string> m_files;
};

} // namespace advcpp

#endif // L32FE72B_FEFD_4357_971D_C6C9620A2ABC_H__
