#include "agentsMng.hpp"
#include "logger.hpp"

#include <exception>
#include <assert.h>

namespace advcpp
{
AgentsMng::AgentsMng() 
{
    
}

void AgentsMng::AddAgent(const std::string& a_ip, SharedPtr<IAgent> a_newAgent) 
{

    a_mapProtect.Lock();
    try{
        m_agentsMap[a_ip] = a_newAgent;
        //std::cerr << "map size after insert :" << m_agentsMap.size() << std::endl;
    }
    catch (const std::exception(& a_ex)){
        a_mapProtect.Unlock(); 
        StartLogger().Exception(a_ex);
        StartLogger().Message("Insert New Agent Failed");  
    }
    a_mapProtect.Unlock();   

}

void AgentsMng::RemoveAgent(const std::string& a_ip) 
{
    a_mapProtect.Lock();
    try{
        m_agentsMap.erase(a_ip);
        //std::cerr << "map size after erase :" << m_agentsMap.size() << std::endl;
    }
    catch (const std::exception(& a_ex)){
        a_mapProtect.Unlock(); 
        StartLogger().Exception(a_ex);
        StartLogger().Message("Erase Agent Failed");  
    }
    a_mapProtect.Unlock();   
}


void AgentsMng::FindAgent(const std::string& a_ip, SharedPtr<IAgent>& a_foundAgent) 
{
    a_mapProtect.Lock();
    try{
        It agentFound = m_agentsMap.find(a_ip);
        if (agentFound == m_agentsMap.end()){
            assert(!"can't be here FindAgent");
        }
        a_foundAgent = agentFound->second;
    }
    catch (const std::exception(& a_ex)){
        a_mapProtect.Unlock(); 
        StartLogger().Exception(a_ex);
        StartLogger().Message("Erase Agent Failed");  
    }
    a_mapProtect.Unlock(); 
    
}
    
} // namespace advcpp
