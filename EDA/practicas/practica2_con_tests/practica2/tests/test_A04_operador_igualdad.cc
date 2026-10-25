#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Track.h"

TEST_CASE("A04: operator==") {
    SUBCASE("Distinto id, mismo título y duración: no son iguales") {
        Track t1("t01", "Uno", 100);
        Track t2("t02", "Uno", 100);
        CHECK_FALSE(t1 == t2);
    }

    SUBCASE("Una pista es igual a sí misma") {
        Track t("t01", "Uno", 100);
        CHECK(t == t);
    }
}
