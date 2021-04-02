#include "func_data.hpp"
#include "cdr_data_mng.hpp"
#include "subscribers_mng.hpp"
#include "logger.hpp"

namespace advcpp
{

SubscriberMngFunc::SubscriberMngFunc(MtWaitingQueue<Subscriber>& a_subQueue, SubscriberData& a_subData)
: m_subQueue(a_subQueue)
, m_subData(a_subData)
{
}   

SubscriberMngFunc::~SubscriberMngFunc()
{
}

void SubscriberMngFunc::operator()()
{
    try{
        SubscribersMng subMng(m_subQueue, m_subData);
        subMng.Start();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread subscriber Mng \n");
        std::terminate();
    }
}

ObjThreadFunc* SubscriberMngFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new SubscriberMngFunc( m_subQueue, m_subData);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}
/**************************************************/


DataMngFunc::DataMngFunc(MtWaitingQueue<Cdr>& a_cdrsQueue)
: m_cdrsQueue(a_cdrsQueue)
{
}   

DataMngFunc::~DataMngFunc()
{
}


void DataMngFunc::operator()()
{
    try{
        DataMng dataMng(m_cdrsQueue);
        dataMng.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread server \n");
        std::terminate();
    }
}

ObjThreadFunc* DataMngFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new DataMngFunc(m_cdrsQueue);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

} // namespace advcpp
