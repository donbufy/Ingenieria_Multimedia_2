#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"

TEST_CASE("B3: findById") {
    SUBCASE("Encuentra un recurso existente entre varios") {
        Catalog c;
        c.add(Resource("bk01", 2000, "Libro 1"));
        c.add(Resource("bk02", 2001, "Libro 2"));
        c.add(Resource("bk03", 2002, "Libro 3"));

        const Resource f = c.findById("bk02");
        REQUIRE(f.getId() != "");
        CHECK(f.getId() == "bk02");
        CHECK(f.getTitle() == "Libro 2");
    }

    SUBCASE("Devuelve recurso vacío si no existe o tras eliminarlo") {
        Catalog c;
        c.add(Resource("bk01", 2000, "Libro"));

        Resource r1 = c.findById("inexistente");
        CHECK(r1.getId() == "");

        c.removeById("bk01");
        Resource r2 = c.findById("bk01");
        CHECK(r2.getId() == "");
    }
}
