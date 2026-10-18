#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A03: Incrementar") {
    Counter c;
    c.increment();
    CHECK(c.getValue() == 1);
    c.increment();
    CHECK(c.getValue() == 2);
}
