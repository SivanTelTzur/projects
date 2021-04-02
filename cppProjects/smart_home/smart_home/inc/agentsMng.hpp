#ifndef BA8E4512_2C86_4C79_96E2_692210BF2849_H__
#define BA8E4512_2C86_4C79_96E2_692210BF2849_H__

//#include "abcAgent.hpp"
#include "iagent.hpp"
#include "mutex.hpp"

#include <tr1/unordered_map>

namespace advcpp
{

class AgentsMng : private NonCopyable
{
public:
    AgentsMng();

    void AddAgent(const std::string& a_ip, SharedPtr<IAgent> a_newAgent);
    void RemoveAgent(const std::string& a_ip);

    void FindAgent(const std::string& a_ip, SharedPtr<IAgent>& a_foundAgent);
private:
    typedef std::tr1::unordered_map<std::string, SharedPtr<IAgent> > UnorderedMap;
    typedef UnorderedMap::iterator It;

private:
    UnorderedMap m_agentsMap; 
    Mutex a_mapProtect;
};

} // namespace advcpp

#endif // BA8E4512_2C86_4C79_96E2_692210BF2849_H__
