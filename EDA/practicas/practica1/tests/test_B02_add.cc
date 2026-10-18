#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"

TEST_CASE("B2: add") {
    SUBCASE("Anade recursos y actualiza el tamano") {
        Catalog c;
        CHECK(c.add(Resource("bk01", 2000, "Libro 1")) == true);
        CHECK(c.add(Resource("bk02", 2001, "Libro 2")) == true);
        CHECK(c.add(Resource("bk03", 2002, "Libro 3")) == true);
        CHECK(c.size() == 3);
    }

    SUBCASE("Rechaza un recurso con id vacío") {
        Catalog c;
        Resource r;
        CHECK(c.add(r) == false);
        CHECK(c.size() == 0);
    }

    SUBCASE("Rechaza un recurso con id duplicado") {
        Catalog c;
        Resource r("bk01", 2000, "Libro");
        CHECK(c.add(r) == true);
        CHECK(c.add(r) == false);
        CHECK(c.size() == 1);
    }
}
