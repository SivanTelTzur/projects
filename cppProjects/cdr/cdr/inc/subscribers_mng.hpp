#ifndef F1B00082_8E53_4E18_A98D_B4C565343354_MNG_H__
#define F1B00082_8E53_4E18_A98D_B4C565343354_MNG_H__

#include "waiting_queue.hpp"
#include "noncopyable.hpp"
#include "subscriber_data.hpp"

#include <string>

namespace advcpp
{
class SubscribersMng : private NonCopyable
{
public:
    static const size_t MAX_SIZE_CONT = 500;

    SubscribersMng(MtWaitingQueue<Subscriber>& a_subscribersQueue, SubscriberData& a_subData);
    ~SubscribersMng();

    void Start();

private:
    void start();
private:
    MtWaitingQueue<Subscriber>&  m_subscribers;
    SubscriberData& m_subData;
};
 
} // namespace advcpp

#endif // F1B00082_8E53_4E18_A98D_B4C565343354_MNG_H__
