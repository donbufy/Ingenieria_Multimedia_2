#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "vector"

TEST_CASE("Z03: memoria"){

    std::vector<int> historial(4);
    historial[0] = 10;
    historial[1] = 20;
    historial[2] = 30;
    historial[3] = 40;

    CHECK(historial[2] == 30);
}