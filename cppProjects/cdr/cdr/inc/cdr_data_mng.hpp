#ifndef U860BD05_42BA_467F_9B05_DBDC57AC2DB7_H__
#define U860BD05_42BA_467F_9B05_DBDC57AC2DB7_H__

#include "waiting_queue.hpp"
#include "cdr_struct.hpp"
#include "noncopyable.hpp"
#include "subscriber_data.hpp"
//#include "cdr_subscriber.hpp" DELETE
namespace advcpp
{
class DataMng : private NonCopyable
{
    static const size_t MAX_SIZE_CONT = 1000;    
public:
    explicit DataMng(MtWaitingQueue<Cdr>& a_cdrQueue);
    ~DataMng();

    void Run();

private:
    MtWaitingQueue<Cdr>& m_cdrs;
    MtWaitingQueue<Subscriber> m_subscribers;
    SubscriberData m_subData;
};

} // namespace advcpp

#endif // U860BD05_42BA_467F_9B05_DBDC57AC2DB7_H__
