#ifndef BE166C5_BDA8_4101_8AB4_0865928E3E78_H__
#define BE166C5_BDA8_4101_8AB4_0865928E3E78_H__

#include "waiting_queue.hpp"
#include "cdr_struct.hpp"
#include "noncopyable.hpp"

#include <string>

namespace advcpp
{
class CdrReceiver : private NonCopyable
{
public:
    static const size_t BUFFER_SIZE = 256;
    static const size_t MAX_QUEUE_SIZE = 1000;

    CdrReceiver(size_t a_port, MtWaitingQueue<Cdr>& a_cdrQueue);
    ~CdrReceiver();

    void Start();

private:
    MtWaitingQueue<Cdr>&  m_queue;
    MtWaitingQueue<std::string> m_bufferQueue;
    size_t m_port;
};   

} // namespace advcpp

#endif // BE166C5_BDA8_4101_8AB4_0865928E3E78_H__
