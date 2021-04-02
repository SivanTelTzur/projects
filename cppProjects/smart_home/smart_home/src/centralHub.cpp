#include "centralHub.hpp"
#include "create_tcp_server.hpp"
#include "server_msgs_recveiver.hpp"
#include "loader.hpp"
#include "routerRunFunction.hpp"
#include "serverRunFunction.hpp"

namespace advcpp
{

CentralHub::CentralHub() 
: m_msgs()
, m_agents()
, m_subscribers(new SubscribersMng())
, m_router(new EventsRouter(*m_subscribers))
, m_server(new ServerHandler(m_msgs, m_agents))
//, m_eventsRecvThread(m_msgs, m_agents)
{
    loadAgents();
    //std::cerr << "I'm in Central Hub Ctor" << std::endl;
    //Loader load(m_router, m_server, m_subscribers, m_agents);
    //load.OpenConfigFile("../inc/smartHomeConfig");
    //SharedPtr<IAgent> agent;
    //m_agents.FindAgent("127.10.10.1", agent);
    //std::cerr << "I'm in Central Hub end Ctor" << "**" <<agent->GetDeviceData().m_config 
    //<< "**" << agent->GetDeviceData().m_config.size()<< std::endl;
    //m_agents.FindAgent("127.10.11.1", agent);
    //std::cerr << "I'm in Central Hub end Ctor" << "**" <<agent->GetDeviceData().m_config
    // << "**" <<agent->GetDeviceData().m_config.size()<< std::endl;
    // //m_eventsRecvThread.Stop();
    
}

CentralHub::~CentralHub() 
{
    delete m_subscribers;
    delete m_router;
    delete m_server;    
}

void CentralHub::loadAgents() 
{
    try{
        Loader load(*m_router, *m_server, *m_subscribers, m_agents);
        load.OpenConfigFile("../inc/smartHomeConfig");          
    }
    catch (const std::exception& a_ex){
        StartLogger().Message("failed to load agents");
        throw ;
    }
}

 


void CentralHub::Run() 
{
    SharedPtr<ObjThreadFunc> threadRouter(new RouterRunFunc(*m_router));
    Thread runRouterThread(threadRouter); //run router thread

    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(*m_server));
    Thread recFromServer(threadServer); //run server thread

    EventMsgReceiveThreadHandler msgThread(m_msgs, m_agents); //msg receiver 

    //Thread::SleepMicroSeconds(10000000);

    //msgThread.Stop();
    runRouterThread.Join();
    recFromServer.Join();
    
}
} // namespace advcpp
