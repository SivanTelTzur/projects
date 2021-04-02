#ifndef AE6A7FD8_81D2_4541_9AC8_46BE33E43A8E_H__
#define AE6A7FD8_81D2_4541_9AC8_46BE33E43A8E_H__

#include "waiting_queue.hpp"
#include "subscriber_data.hpp"
#include "query.hpp"
#include "noncopyable.hpp"
#include "inet_server.hpp"

namespace advcpp
{

class QueryGateway : private NonCopyable
{
    static const size_t MAX_SIZE_CONT = 120;    
    static const size_t PORT = 8080; 
public:
    explicit QueryGateway(SubscriberData& a_subData);
    ~QueryGateway();

    void Run();

private:
    // void startListenServerThread();
    void queryParser(const Query& a_query , std::string& a_action, std::string& a_key) const; // return action and key seperated by |
    void msisdnQueryResult(Query& a_query, std::string& a_key);
    void queryNevigator(Query& a_query , std::string& a_action, std::string& a_key); // return an altered query with answer ready to be sent
    //void startCdrSenderNetTheard();


private:
    SubscriberData& m_subData;
    MtWaitingQueue<Query> m_queries;
    INetServer& m_queryServer;
};

} // namespace advcpp

#endif // AE6A7FD8_81D2_4541_9AC8_46BE33E43A8E_H__
