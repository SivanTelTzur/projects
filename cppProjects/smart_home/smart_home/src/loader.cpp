#include "loader.hpp"
#include "logger.hpp"
#include "agentsMng.hpp"

#include <exception>
#include <dlfcn.h> //dlopen  

namespace advcpp
{
Loader::Loader(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription, AgentsMng& a_agentsMng) 
: m_router(a_router)
, m_server(a_server)
, m_subscription(a_subscription)
, m_agentsMng(a_agentsMng)
, m_fp()
{
    std::cerr << "I'm in Loader Ctor" << std::endl;    
}

Loader::~Loader()
{
    m_fp.close();
}

void Loader::OpenConfigFile(const std::string& a_fileName) 
{
    m_fp.open(a_fileName.c_str(), std::ifstream::in); 
    if (!m_fp.is_open()){
        StartLogger().Message("failed to open file");
        throw (std::runtime_error("failed to open file"));
    }
    std::cerr << "I'm in Loader CtorOpenConfigFile" << std::endl;
    parseConfigFile();
}

void Loader::parseConfigFile() 
{
    Config deviceConfig; 
    std::string* device[NUM_OF_CONFIG_VALUES] = {&deviceConfig.m_ip, &deviceConfig.m_type
                                                , &deviceConfig.m_room, &deviceConfig.m_floor
                                                , &deviceConfig.m_log, &deviceConfig.m_config};
    while(!m_fp.eof()){
        std::string line;
        for (size_t i = 0; i < NUM_OF_CONFIG_VALUES; ++i){
            std::getline(m_fp, line);
            updateDeviceMember(line, *device[i], i);
        }
        createAgentFromSo(deviceConfig);
        std::getline(m_fp, line); //skip empty line
        deviceConfig.clearAll();
    }
}

void Loader::updateDeviceMember(const std::string& a_line, std::string& a_member, size_t a_memPlaceNum) 
{
    if (a_memPlaceNum == 0){
        a_member.append(a_line, 1, a_line.size() - 2); //reduce []
        std::cerr << "ip updateDeviceMember :" << a_member << std::endl;
        return;
    }
    size_t startPos = a_line.find_first_of("=");
    startPos = a_line.find_first_not_of(' ', startPos + 1); //avoid space if exist
    size_t endPos = a_line.find_last_not_of(" \n\t\r\f\v"); //end of string pos
    if (startPos > endPos){ //case empty
        return;
    }
    a_member.append(a_line, startPos, endPos - startPos + 1); 
}

int Loader::createAgentFromSo(Config& a_deviceConfig) 
{
    std::string soFile = a_deviceConfig.m_type + ".so"; //"../sofiles/" + 

    void* module = ::dlopen(soFile.c_str(), RTLD_LAZY);
    if(!module){
        std::cerr << "couldn't load dynamic lib: " << soFile << '\n';
        std::cerr << ::dlerror() << '\n';
        return -2;
    }else{
        std::cout << soFile.c_str() << " was loaded to address: " << module << '\n';
    }

    void* psym = ::dlsym(module, "CreateMeAnAgent");
    if(!psym){
        std::cerr << "couldn't locate symbol: " << "CreateMeAnAgent" << '\n';
        std::cerr << ::dlerror() << '\n';
        return -1;
    }else{
        std::cout << "Got pointer to symbol: " << psym << '\n';
    }
    
    //typedef IAgent* (*AgentCreate)(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription);
    CreateAgent pf = (CreateAgent)(psym);
    SharedPtr<IAgent> agent(pf(m_router, m_server, m_subscription));
    std::cerr << "I'm in Loader  after Ctor agent address " << &agent <<  std::endl;    
    agent->InitAgent(a_deviceConfig, agent); // subscription made by the agent
    m_agentsMng.AddAgent(a_deviceConfig.m_ip, agent);

    return 0;
}
    

} // namespace advcpp
