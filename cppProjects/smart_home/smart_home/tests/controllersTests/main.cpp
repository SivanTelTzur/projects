#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <tr1/unordered_map>



#include "sleep_micro_sec.hpp"
#include "thread_group.hpp"
#include "serverRunFunction.hpp" //ServerRecvFunc
#include "routerRunFunction.hpp" //RouterRunFunc
#include "server_msgs_recveiver.hpp"
#include "TemperatureAgent.hpp"
#include "agentsMng.hpp"
#include "sensorsHandler.hpp"
#include "serverHandler.hpp"
#include "eventsRouter.hpp"
#include "subscribersMng.hpp"


using namespace advcpp;

BEGIN_TEST(SingleMsgSent)
    TemperatureAgent::ResetCounter();
    SubscribersMng subscribers;
    EventsRouter router(subscribers);

    SharedPtr<ObjThreadFunc> threadRouter(new RouterRunFunc(router));
    Thread runRouterThread(threadRouter); //run router thread

    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng devices;
    ServerHandler server(msgQueue, devices);

    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
    Thread recFromServer(threadServer); //run server thread

    SharedPtr<IAgent> aSensor(CreateMeAnAgent(router, server, subscribers));
    SharedPtr<IAgent> bSensor(CreateMeAnAgent(router, server, subscribers));
    SharedPtr<IAgent> controller(CreateMeAnAgent(router, server, subscribers));

    Config aData = {"127.10.10.1", "ambient_temp", "room_8_b", "1", "m_log", "m_config", 0};
    aSensor->InitAgent(aData, aSensor);
    Config bData = {"127.10.11.1", "ambient_temp", "room_9_b", "1", "m_log", "m_config", 0};
    bSensor->InitAgent(bData, bSensor);
    Config cData = {"127.10.20.1", "AC-Tadiran", "room_9_b", "1", "m_log", "m_config", 0};
    controller->InitAgent(cData, controller);

    devices.AddAgent(aSensor->GetDeviceData().m_ip.c_str(), aSensor);
    devices.AddAgent(bSensor->GetDeviceData().m_ip.c_str(), bSensor);
    devices.AddAgent(controller->GetDeviceData().m_ip.c_str(), controller);

    Topic topic = {"LOW_TEMP",{"room_9_b", "1"}};
    controller->Subscribe(topic, controller);

    EventMsgReceiveThreadHandler messages(msgQueue, devices);

    recFromServer.Join();
    runRouterThread.Join();
    messages.Stop();
 
    ASSERT_PASS();
END_TEST


TEST_SUITE(Test system)
    TEST(SingleMsgSent)
END_SUITE
