#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A04: Decrementar") {
    Counter c(3);
    CHECK(c.decrement() == true);
    CHECK(c.getValue() == 2);
    CHECK(c.decrement() == true);
    CHECK(c.getValue() == 1);
}
