#include "query_gateway.hpp"
#include "create_query_server.hpp"
#include "func_server.hpp"
#include "thread.hpp"

#include <sstream> //ostringstream
#include <string.h>
#include <cstdio> // sprintf

namespace advcpp
{

QueryGateway::QueryGateway(SubscriberData& a_subData) 
: m_subData(a_subData)
, m_queries(MAX_SIZE_CONT)
, m_queryServer(CreateQueriesServer(PORT, m_queries))
{    
}  

QueryGateway::~QueryGateway() 
{
    delete &m_queryServer;    
}

void QueryGateway::Run() 
{
    //create thread for listing 
    ServerRcvFunc serverFunc(m_queryServer); 
    Thread serverRcv(serverFunc);

    for (;;){
        Query newQuery = m_queries.Dequeue();
        std::string action;
        std::string key;
        queryParser(newQuery, action, key);
        queryNevigator(newQuery, action, key);
        m_queryServer.Send(newQuery.query.c_str(), newQuery.query.size() + 1, newQuery.socket);
    }
    serverRcv.Join();
    delete &serverFunc;
    
}

void QueryGateway::queryNevigator(Query& a_query , std::string& a_action, std::string& a_key) 
{
    if (a_action == "msisdn"){
        msisdnQueryResult(a_query, a_key);
    }    
}


void QueryGateway::queryParser(const Query& a_query , std::string& a_action, std::string& a_key) const
{
    size_t pos = a_query.query.find("query");
    pos = a_query.query.find("/", pos + 4 ); //"query = 4"
    size_t start = pos + 1; //action starts
    pos = a_query.query.find("/", start );
    a_action = a_query.query.substr(start, pos - start);
    start = pos + 1;
    pos = a_query.query.find_first_of(" /", start);
    a_key = a_query.query.substr(start, pos - start);
}

void QueryGateway::msisdnQueryResult(Query& a_query, std::string& a_key) 
{
    Subscriber result = m_subData.GetValue(a_key);
    const char delimeter = '"';
    std::ostringstream osResult;
    
    osResult << "{" << delimeter << "msisdn" << delimeter << ":" << delimeter << result.msisdn << delimeter << ",\n";
    osResult << delimeter << "voice-out" << delimeter << ":" << result.voiceOut << ",\n";
    osResult << delimeter << "voice-in" << delimeter << ":" << result.voiceIn << ",\n";
    osResult << delimeter << "sms-out" << delimeter << ":" << result.smsOut << ",\n";
    osResult << delimeter << "sms-in" << delimeter << ":" << result.smsIn << ",\n";
    osResult << delimeter << "data-out" << delimeter << ":" << result.dataOut << ",\n";
    osResult << delimeter << "data-in" << delimeter << ":" << result.dataIn << "\n}";


    std::ostringstream json;
    json << "HTTP/1.1 200 OK\r\n";
    json << "Cache-Control: private\r\n";
    json << "Content-Type: application/json; charset=utf-8\r\n";
    std::string answer = osResult.str();
    json << "Content-Length: " << answer.size() << "\r\n" ;
    json << "Connection: Close\r\n\r\n";
    json << answer;

    answer = json.str();    
    a_query.query = answer;

}

} // namespace advcpp
