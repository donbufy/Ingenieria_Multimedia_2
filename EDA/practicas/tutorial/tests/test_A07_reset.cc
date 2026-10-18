#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A07: Reset") {
    Counter c(42);

    c.reset();
    
    CHECK(c.getValue() == 0);
    
    
}

