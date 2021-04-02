#include "cdr_handler.hpp"
#include "mu_test.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>//sleep

using namespace advcpp;

UNIT(cdr_handler_run)
    CdrHandler s;
    s.Run();
    sleep(20);
END_UNIT

TEST_SUITE(Test class cdr_handler)
    TEST(cdr_handler_run)
END_SUITE