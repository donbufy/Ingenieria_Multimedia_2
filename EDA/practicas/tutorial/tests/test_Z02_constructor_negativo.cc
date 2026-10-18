#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("Z02: constructor_desde_negativo"){

    Counter c(-10);

    CHECK(c.getValue() == -10);

    c.increment();

    CHECK(c.getValue() == -9);

    c.decrement();

    CHECK(c.getValue() == -9);
 
}