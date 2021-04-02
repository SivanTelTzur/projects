#include "mu_test_new.h"

#include <iostream>
#include <stdlib.h> //rand
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <tr1/unordered_map>


#include "centralHub.hpp"

using namespace advcpp;

BEGIN_TEST(loadHub)
    CentralHub x; 
    x.Run();    
    ASSERT_PASS();
END_TEST


TEST_SUITE(Test centralHub)
    TEST(loadHub)
END_SUITE
