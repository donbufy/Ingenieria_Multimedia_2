#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Resource.h"

TEST_CASE("A4: operator== y operator<") {
    SUBCASE("operator== compara solo por id") {
        Resource r1("bk01", 1999, "Clean Code");
        Resource r2("bk01", 2020, "Otro titulo");
        Resource r3("bk02", 1999, "Clean Code");
        CHECK(r1 == r2);
        CHECK_FALSE(r1 == r3);
    }

    SUBCASE("operator< compara por id") {
        Resource r1("aaa", 2000, "Primero");
        Resource r2("bbb", 2000, "Segundo");
        CHECK(r1 < r2);
        CHECK_FALSE(r2 < r1);
        CHECK_FALSE(r1 < r1);
    }
}
