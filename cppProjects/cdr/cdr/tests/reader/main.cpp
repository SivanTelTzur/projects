#include "mu_test.h"
#include "reader.hpp"
#include "cdr_struct.hpp"
#include "waiting_queue.hpp"

using namespace advcpp;

UNIT(Reader_create)
    MtWaitingQueue<Cdr> myque(0);
    Reader reader(myque, "test-10.cdr");
    reader.Start();
    ASSERT_THAT(!myque.IsEmpty());
END_UNIT

TEST_SUITE(Test class Thread)
    TEST(Reader_create)
END_SUITE
