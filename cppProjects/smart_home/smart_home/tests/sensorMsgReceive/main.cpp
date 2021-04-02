#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <tr1/unordered_map>


#include "waitableQueue.hpp"
#include "sleep_micro_sec.hpp"
#include "thread_group.hpp"
#include "create_tcp_server.hpp"
#include "server_new_client_receiver.hpp"
#include "Imsg_handler_func.hpp"
#include "inew_client_action.hpp"
#include "serverTestFunctions.hpp"
#include "server_msgs_recveiver.hpp"
#include "agent.hpp"
#include "agentsMng.hpp"
#include "sensorsHandler.hpp"
#include "serverHandler.hpp"
#include "eventsRouter.hpp"


//static const int QUEUE_SIZE = 100;


using namespace advcpp;

BEGIN_TEST(SensorCreateAgent)
    Agent::ResetCounter();
    //SharedPtr<INetServer> server;
    WaitableQueue<SensorMsg> msgQueue;
    std::tr1::unordered_multimap<Topic, SharedPtr<AbcAgent> > UnorderedMap;
    WaitableQueue<Event> EventQueue;
    EventsRouter dummy(UnorderedMap, EventQueue);
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);

    Agent x(dummy, server);
    x.Notify("test");
    ASSERT_EQUAL(Agent::GetCounter(), 1);  
    ASSERT_PASS();
END_TEST

BEGIN_TEST(SensorDeviceMngCreate)
    Agent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);
    std::tr1::unordered_multimap<Topic, SharedPtr<AbcAgent> > UnorderedMap;
    WaitableQueue<Event> EventQueue;
    EventsRouter dummy(UnorderedMap, EventQueue);

    Agent x(dummy, server);


    x.Notify("test");
//    server->Run();
    ASSERT_EQUAL(Agent::GetCounter(), 1);  
END_TEST

BEGIN_TEST(SensorDeviceMngInsertAgent)
    Agent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
        std::cerr << "I'm here" << std::endl;
    ServerHandler server(msgQueue, devices);
    std::tr1::unordered_multimap<Topic, SharedPtr<AbcAgent> > UnorderedMap;
    WaitableQueue<Event> EventQueue;
    EventsRouter dummy(UnorderedMap, EventQueue);

    SharedPtr<AbcAgent> y(new Agent(dummy, server));
    //AgentsMng devices;
    Config data = {"127.10.10.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", 14, 1, 0};
    y->InitAgent(data);
    devices.AddAgent(y->m_deviceData.m_ip.c_str(), y);
    y->Notify("test");
    ASSERT_EQUAL(Agent::GetCounter(), 1);    
    SharedPtr<AbcAgent> temp;
    devices.FindAgent(y->m_deviceData.m_ip.c_str(),temp);
    std::cerr << temp->m_deviceData.m_log << std::endl;
    devices.RemoveAgent(y->m_deviceData.m_ip.c_str());
//    server->Run();
    ASSERT_PASS();
END_TEST

BEGIN_TEST(SensorReceiveMsgServerClient)
    Agent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);
    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
    Thread recFromServer(threadServer);
    std::tr1::unordered_multimap<Topic, SharedPtr<AbcAgent> > UnorderedMap;
    WaitableQueue<Event> EventQueue;
    EventsRouter dummy(UnorderedMap, EventQueue);

    SharedPtr<AbcAgent> y(new Agent(dummy, server));
    Config data = {"127.10.10.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", 14, 1, 0};
    y->InitAgent(data);
    devices.AddAgent(y->m_deviceData.m_ip.c_str(), y);
    SharedPtr<AbcAgent> x(new Agent(dummy, server));
    Config datax = {"127.10.11.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", 14, 1, 0};
    x->InitAgent(datax);
    devices.AddAgent(x->m_deviceData.m_ip.c_str(), x);

    EventMsgReceiveThreadHandler messages(msgQueue, devices);

    recFromServer.Join();
    messages.Stop();
    ASSERT_PASS();
END_TEST

//BEGIN_TEST(ServerThreadTest)
//    SharedPtr<IMsgHandlerFunc> getMsg(new TestRcvFunction);
//    SharedPtr<INetServer> server(CreateTcpServer(1200, getMsg));
//    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
//    Thread recFromServer(threadServer);
//    recFromServer.Join();
//    ASSERT_PASS();
//END_TEST    
//
//BEGIN_TEST(ServerThreadInsertToQueTest)
//    WaitableQueue<SensorMsg> msgQueue;
//    SharedPtr<IMsgHandlerFunc> getMsg(new ServerMsgsReceiver(msgQueue));
//    SharedPtr<INetServer> server(CreateTcpServer(1200, getMsg));
//    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
//    Thread recFromServer(threadServer);
//    recFromServer.Join();
//    ASSERT_PASS();
//END_TEST   

TEST_SUITE(Test server)
    TEST(SensorCreateAgent)
    TEST(SensorDeviceMngCreate)
    TEST(SensorDeviceMngInsertAgent)
    TEST(SensorReceiveMsgServerClient)
    //IGNORE_TEST(ServerThreadTest)
    //IGNORE_TEST(ServerThreadInsertToQueTest)
END_SUITE
