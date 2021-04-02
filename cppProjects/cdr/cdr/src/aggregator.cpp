#include "aggregator.hpp"
#include "thread.hpp"
#include "func_server.hpp"
#include "func_data.hpp"
#include "logger.hpp"

namespace advcpp
{
Aggregator::Aggregator()
: m_cdrs(MAX_SIZE_CONT)
{
}

Aggregator::~Aggregator() 
{ 
}

void Aggregator::Run() 
{
    startReceiverThread();    
}

void Aggregator::startReceiverThread() 
{
    try{
        ReceiverFunc receiverFunc(PORT, m_cdrs);
        Thread receiverThread(receiverFunc);
        
        DataMngFunc dataFunc(m_cdrs);
        Thread dataMngThread(dataFunc);
        receiverThread.Join();
        dataMngThread.Join();
    } 
    catch (const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("receiver Thread");
    }
    catch (...){
        StartLogger().Message("startReceiverThread throw unknown");
    }    

    //try{
        //DataMngFunc dataFunc(m_cdrs);
        //Thread dataMngThread(dataFunc);
    //    dataMngThread.Join();
    //} 
    //catch (const std::exception& a_ex){
    //    StartLogger().Exception(a_ex);
    //    StartLogger().Message("receiver Thread");
    //}
    //catch (...){
    //    StartLogger().Message("startReceiverThread throw unknown");
    //}   

}

} // namespace advcpp
