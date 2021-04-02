#include "cdr_data_mng.hpp"
#include "logger.hpp"
#include "cdr_converter.hpp"
#include "thread.hpp"
#include "func_data.hpp"
#include "func_query.hpp"

#include <exception>

namespace advcpp
{
DataMng::DataMng(MtWaitingQueue<Cdr>& a_cdrQueue) 
: m_cdrs(a_cdrQueue)
, m_subscribers(MAX_SIZE_CONT)
, m_subData()
{    
}

DataMng::~DataMng() 
{
    
}

void DataMng::Run() 
{
    SubscriberMngFunc subscribersFunc(m_subscribers, m_subData);
    Thread subscribersDataThread(subscribersFunc);

    QueryGatewayFunc queryGatewayFunc(m_subData);
    Thread queryGatewayThread(queryGatewayFunc);

    for (; ;){
        try{
            Cdr newCdr;
            newCdr = m_cdrs.Dequeue();
            m_subscribers.Enqueue(cdr_converter::CdrToSubscriber(newCdr));

        }
        catch (const std::exception& a_ex){
            StartLogger().Message( "protocol sent exception" );
            StartLogger().Exception(a_ex);
        }
        
    }
    subscribersDataThread.Join();
    queryGatewayThread.Join();

}

} // namespace advcpp
