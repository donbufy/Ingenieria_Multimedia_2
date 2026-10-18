#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"
#include <sstream>

TEST_CASE("A06: Salida con operator<<") {
    Counter c(5);
    std::ostringstream oss;
    oss << c;
    CHECK(oss.str() == "Counter: 5");
}
