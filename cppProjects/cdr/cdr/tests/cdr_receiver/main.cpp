#include "cdr_receiver.hpp"
#include "mu_test.h"
#include "waiting_queue.hpp"
#include "cdr_struct.hpp"

#include <iostream>
#include <stdio.h>
#include <unistd.h>//sleep

using namespace advcpp;

UNIT(cdr_receiver_run)
    MtWaitingQueue<Cdr> a_cdrQueue(0);
    CdrReceiver s(1200, a_cdrQueue);
    s.Start();
    sleep(20);
END_UNIT

TEST_SUITE(Test class cdr_receiver)
    TEST(cdr_receiver_run)
END_SUITE