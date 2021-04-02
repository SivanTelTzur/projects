#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <ctime>


#include "sleep_micro_sec.hpp"
#include "create_tcp_client.hpp"
#include "inet_client.hpp"
#include "sampleController.hpp"



using namespace advcpp;


BEGIN_TEST(client_send)
    Controller client("127.10.20.1", 1200);
    client.Run();
    ASSERT_PASS();
END_TEST



TEST_SUITE(client_tcp)
    TEST(client_send)   
END_SUITE
