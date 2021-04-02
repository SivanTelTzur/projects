#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>


#include "waitableQueue.hpp"
#include "sleep_micro_sec.hpp"
#include "thread_group.hpp"
#include "create_tcp_server.hpp"
#include "serverRcvFunction.hpp"
#include "Imsg_handler_func.hpp"
#include "serverTestFunctions.hpp"
#include "server_msgs_recveiver.hpp"
#include "serverHandler.hpp"
#include "agentsMng.hpp"
#include "inew_client_action.hpp"
#include "server_new_client_receiver.hpp"



//static const int QUEUE_SIZE = 100;

using namespace advcpp;

BEGIN_TEST(ServerPlainTest)
    SharedPtr<IMsgHandlerFunc> getMsg(new TestRcvFunction);
    SharedPtr<INewClientHandlerFunc> dummy(new TestDummyFunction);
    
    INetServer* server = CreateTcpServer(1200, getMsg, dummy);
    server->Run();
    ASSERT_PASS();
END_TEST

BEGIN_TEST(ServerThreadTest)
    SharedPtr<IMsgHandlerFunc> getMsg(new TestRcvFunction);
    SharedPtr<INewClientHandlerFunc> dummy(new TestDummyFunction);
    SharedPtr<INetServer> server(CreateTcpServer(1200, getMsg, dummy));
    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
    Thread recFromServer(threadServer);
    recFromServer.Join();
    ASSERT_PASS();
END_TEST    

BEGIN_TEST(ServerThreadInsertToQueTest)
    WaitableQueue<SensorMsg> msgQueue;
    SharedPtr<IMsgHandlerFunc> getMsg(new ServerMsgsReceiver(msgQueue));
    SharedPtr<INewClientHandlerFunc> dummy(new TestDummyFunction);
    SharedPtr<INetServer> server(CreateTcpServer(1200, getMsg, dummy));
    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFunc(server));
    Thread recFromServer(threadServer);
    recFromServer.Join();
    ASSERT_PASS();
END_TEST  

/*can't be tested ! Must have agents exist to activate handler*/
BEGIN_TEST(ServerHandlerThreadInsertToQueTest)
    WaitableQueue<SensorMsg> msgQueue;
    AgentsMng agentsMng;
    ServerHandler server(msgQueue, agentsMng); 
    SharedPtr<ObjThreadFunc> threadServer(new ServerRecvFuncWithHandler(server));
    Thread recFromServer(threadServer);
    recFromServer.Join();
    ASSERT_PASS();
END_TEST   

TEST_SUITE(Test server)
    IGNORE_TEST(ServerPlainTest)
    IGNORE_TEST(ServerThreadTest)
    IGNORE_TEST(ServerThreadInsertToQueTest)
    TEST(ServerHandlerThreadInsertToQueTest)
END_SUITE
