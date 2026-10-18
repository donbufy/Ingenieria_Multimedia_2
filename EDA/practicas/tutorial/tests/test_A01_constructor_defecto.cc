#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A01: Constructor por defecto") {
    Counter c;
    CHECK(c.getValue() == 0);
}
