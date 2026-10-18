#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A02: Constructor con parametros") {
    Counter c(5);
    CHECK(c.getValue() == 5);
}
