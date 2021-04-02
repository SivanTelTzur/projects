#ifndef CA73728A_4AEE_437A_8F09_E9BF0A31012C_H__
#define CA73728A_4AEE_437A_8F09_E9BF0A31012C_H__

#include "isubscription.hpp"
#include "noncopyable.hpp"
#include "event.hpp"
//#include "iagent.hpp"
#include "hashFunc.hpp"
#include "mutex.hpp"

#include <tr1/unordered_map>


namespace advcpp
{

class SubscribersMng : public ISubscription, private NonCopyable
{
    typedef std::tr1::unordered_multimap<Topic, SharedPtr<IAgent>, HashFunction> UnorderedMap;
    typedef UnorderedMap::iterator It;
    typedef UnorderedMap::const_iterator CIt;
    
public:
    SubscribersMng();
    ~SubscribersMng();


    virtual void Subscribe(const Topic a_topic, SharedPtr<IAgent> a_Agent);
    void Unsubscribe(const Topic a_topic, SharedPtr<IAgent> a_Agent);
    void UpdateSubscribers(Event a_newEvent)const;

private:
    void addSubscriber(const Topic a_topic, SharedPtr<IAgent> a_Agent);
    void removeSubscriber(const Topic a_topic, SharedPtr<IAgent> a_Agent);
    void updateSubscribers(Event a_newEvent)const;

private:
    UnorderedMap m_subscribers;
    mutable Mutex m_mapMtx;
};

} // namespace advcpp

#endif // CA73728A_4AEE_437A_8F09_E9BF0A31012C_H__
