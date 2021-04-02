#include "mu_test.h"
#include "create_parser.hpp"

#include <stdio.h>
#include <iostream>

using namespace advcpp;

UNIT(parser_test_create)
    IParser& parser = CreateParser();
    Cdr cdr;
    parser.Parser("0123|56|89|0123456|890123|456|8|0|2|4", cdr);
    ASSERT_THAT(*cdr.pValues[0] == "56");
    ASSERT_THAT(*cdr.pValues[1] == "89");
    ASSERT_THAT(*cdr.pValues[2] == "0123456");
    ASSERT_THAT(*cdr.pValues[3] == "890123");
    ASSERT_THAT(*cdr.pValues[4] == "456");
    ASSERT_THAT(*cdr.pValues[5] == "8");
    ASSERT_THAT(*cdr.pValues[6] == "0");
    ASSERT_THAT(*cdr.pValues[7] == "2");
    ASSERT_THAT(*cdr.pValues[8] == "4");
END_UNIT

TEST_SUITE(Test Parser)
    TEST(parser_test_create)
END_SUITE