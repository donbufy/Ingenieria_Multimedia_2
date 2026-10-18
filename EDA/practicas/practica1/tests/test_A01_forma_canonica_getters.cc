#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Resource.h"

TEST_CASE("A1: Forma canonica y getters") {
    SUBCASE("Constructor por defecto") {
        Resource r;
        CHECK(r.getId() == "");
        CHECK(r.getYear() == 0);
        CHECK(r.getTitle() == "");
        CHECK(r.getTags().empty());
    }

    SUBCASE("Constructor con parametros") {
        Resource r("bk01", 1999, "Clean Code");
        CHECK(r.getId() == "bk01");
        CHECK(r.getYear() == 1999);
        CHECK(r.getTitle() == "Clean Code");
        CHECK(r.getTags().empty());
    }

    SUBCASE("Constructor copia y operador asignacion") {
        Resource r1("bk01", 1999, "Clean Code");
        Resource r2(r1);
        Resource r3;
        r3 = r1;

        CHECK(r2.getId() == "bk01");
        CHECK(r3.getId() == "bk01");
    }
}
