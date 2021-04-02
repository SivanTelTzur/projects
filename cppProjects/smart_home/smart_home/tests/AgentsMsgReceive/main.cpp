#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <tr1/unordered_map>



#include "sleep_micro_sec.hpp"
#include "thread_group.hpp"
#include "serverTestFunctions.hpp"
#include "server_msgs_recveiver.hpp"
#include "TemperatureAgent.hpp"
#include "agentsMng.hpp"
#include "sensorsHandler.hpp"
#include "serverHandler.hpp"
#include "eventsRouter.hpp"
#include "subscribersMng.hpp"


//static const int QUEUE_SIZE = 100;


using namespace advcpp;

BEGIN_TEST(SensorCreateAgent)
    TemperatureAgent::ResetCounter();
    SubscribersMng subscribers;
    EventsRouter dummy(subscribers);
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);

    TemperatureAgent x(dummy, server, subscribers);
    x.testMe();
    ASSERT_EQUAL(TemperatureAgent::GetCounter(), 1);  
    ASSERT_PASS();
END_TEST

BEGIN_TEST(SensorDeviceMngCreate)
    TemperatureAgent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);
    SubscribersMng subscribers;
    EventsRouter dummy(subscribers);

    TemperatureAgent x(dummy, server, subscribers);

    x.testMe();

    ASSERT_EQUAL(TemperatureAgent::GetCounter(), 1);  
END_TEST

BEGIN_TEST(SensorDeviceMngInsertAgent)
    TemperatureAgent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);
        //std::cerr << "I'm here" << std::endl;
    SubscribersMng subscribers;
    EventsRouter dummy(subscribers);

    SharedPtr<IAgent> y(CreateMeAnAgent(dummy, server, subscribers));

    Config data = {"127.10.10.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", "room_8_b", "1", 0};
    y->InitAgent(data);
    devices.AddAgent(y->GetDeviceData().m_ip.c_str(), y);
    y->testMe();
    ASSERT_EQUAL(TemperatureAgent::GetCounter(), 1);    
    SharedPtr<IAgent> temp;
    devices.FindAgent(y->GetDeviceData().m_ip.c_str(),temp);
    std::cerr << temp->GetDeviceData().m_log << std::endl;
    temp->testMe();
    ASSERT_EQUAL(TemperatureAgent::GetCounter(), 2);    
    devices.RemoveAgent(y->GetDeviceData().m_ip.c_str());
    ASSERT_PASS();
END_TEST

BEGIN_TEST(SensorReceiveMsgServerClient)
    TemperatureAgent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);
    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
    Thread recFromServer(threadServer);

    SubscribersMng subscribers;
    EventsRouter dummy(subscribers);

    SharedPtr<IAgent> y(CreateMeAnAgent(dummy, server, subscribers));
    Config data = {"127.10.10.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", "room_10_b", "1", 0};
    y->InitAgent(data);
    devices.AddAgent(y->GetDeviceData().m_ip.c_str(), y);
    SharedPtr<IAgent> x(CreateMeAnAgent(dummy, server, subscribers));
    Config datax = {"127.10.11.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", "room_8_l", "1", 0};
    x->InitAgent(datax);
    devices.AddAgent(x->GetDeviceData().m_ip.c_str(), x);
    Topic topic = {"ambient_temp", {"room_8_l", "1"}};
    x->Subscribe(topic, x);

    EventMsgReceiveThreadHandler messages(msgQueue, devices);

    recFromServer.Join();
    messages.Stop();
    ASSERT_PASS();
END_TEST

BEGIN_TEST(SubscribersMngTest)
    TemperatureAgent::ResetCounter();
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);

    SubscribersMng subscribers;
    EventsRouter dummy(subscribers);

    SharedPtr<IAgent> y(CreateMeAnAgent(dummy, server, subscribers));
    Config data = {"127.10.10.1", "Temperature-1-a", "HeatAlert", "m_log", "m_config", "room_8_b", "7", 0};
    y->InitAgent(data);
    devices.AddAgent(y->GetDeviceData().m_ip.c_str(), y);
    SharedPtr<IAgent> x(CreateMeAnAgent(dummy, server, subscribers));
    Config datax = {"127.10.11.1", "Temperature-1-a", "ambient_temp", "m_log", "m_config", "room_1_b", "1", 0};
    x->InitAgent(datax);
    devices.AddAgent(x->GetDeviceData().m_ip.c_str(), x);
    Topic topic = {"ambient_temp", {"room_8_l", "1"}};
    x->Subscribe(topic, x);
    topic.m_eventType = "HeatAlert";
    topic.m_location.m_floor = "14";
    topic.m_location.m_room = "room_1_a";
    x->Subscribe(topic, x);
    y->Subscribe(topic, y);
    Event event= {topic, "timestamp", ""};
    subscribers.UpdateSubscribers(event);
    topic.m_location.m_floor = "9";
    y->Subscribe(topic, y);
    ASSERT_PASS();
END_TEST


TEST_SUITE(Test server)
    TEST(SensorCreateAgent)
    TEST(SensorDeviceMngCreate)
    TEST(SensorDeviceMngInsertAgent)
    IGNORE_TEST(SensorReceiveMsgServerClient)
    TEST(SubscribersMngTest)

END_SUITE
