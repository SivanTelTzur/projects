#ifndef FE26D740_149D_41E3_8FF3_C6AD24C51495_H__
#define FE26D740_149D_41E3_8FF3_C6AD24C51495_H__

#include "waiting_queue.hpp"
#include "cdr_struct.hpp"

namespace advcpp
{
class Aggregator : private NonCopyable
{
static const size_t PORT = 1200; 
static const size_t MAX_SIZE_CONT = 1000; 
public:
    Aggregator();
    ~Aggregator();

    void Run();

private:
    void startReceiverThread();


private:
    MtWaitingQueue<Cdr> m_cdrs;

};

} // namespace advcpp
#endif // FE26D740_149D_41E3_8FF3_C6AD24C51495_H__
