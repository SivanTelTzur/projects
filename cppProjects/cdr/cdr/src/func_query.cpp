#include "func_query.hpp"
#include "query_gateway.hpp"
#include "logger.hpp"

namespace advcpp
{

QueryGatewayFunc::QueryGatewayFunc(SubscriberData& a_subData)
: m_subData(a_subData)
{
}   

QueryGatewayFunc::~QueryGatewayFunc()
{
}

void QueryGatewayFunc::operator()()
{
    try{
        QueryGateway subMng(m_subData);
        subMng.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread subscriber Mng \n");
        std::terminate();
    }
}

ObjThreadFunc* QueryGatewayFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new QueryGatewayFunc(m_subData);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

}//advcpp
