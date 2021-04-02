#include "mu_test.h"
#include "files_notifier.hpp"
#include "waiting_queue.hpp"

#include <stdio.h>

using namespace advcpp;

UNIT(Notifier_create)
    MtWaitingQueue<std::string> myque(0);
    FilesNotifier dir(myque, "./files/");
    dir.Run();
    ASSERT_THAT(!myque.IsEmpty());
END_UNIT

TEST_SUITE(Test class notifier)
    TEST(Notifier_create)
END_SUITE
