#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("Z01: Incrementar desde valor inicial") {

    Counter c(10);
    c.increment();
    CHECK(c.getValue() == 11);

}