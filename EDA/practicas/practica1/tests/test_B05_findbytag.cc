#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"
#include <vector>

TEST_CASE("B5: findByTag") {
    SUBCASE("Encuentra recursos con esa etiqueta y normaliza la busqueda") {
        Catalog c;
        Resource r1("bk01", 2000, "Libro 1");
        r1.addTag("cpp");
        Resource r2("bk02", 2001, "Libro 2");
        r2.addTag("cpp");
        r2.addTag("java");
        Resource r3("bk03", 2002, "Libro 3");
        r3.addTag("java");

        c.add(r1);
        c.add(r2);
        c.add(r3);

        std::vector<Resource> res = c.findByTag("CPP");
        REQUIRE(res.size() == 2);
        CHECK(res[0].getId() == "bk01");
        CHECK(res[1].getId() == "bk02");
    }

    SUBCASE("Devuelve los resultados ordenados por id") {
        Catalog c;
        Resource r1("zzz", 2002, "Ultimo");
        r1.addTag("tag1");
        Resource r2("aaa", 2000, "Primero");
        r2.addTag("tag1");
        Resource r3("mmm", 2001, "Medio");
        r3.addTag("tag1");

        c.add(r1);
        c.add(r2);
        c.add(r3);

        std::vector<Resource> res = c.findByTag("tag1");
        REQUIRE(res.size() == 3);
        CHECK(res[0].getId() == "aaa");
        CHECK(res[1].getId() == "mmm");
        CHECK(res[2].getId() == "zzz");
    }

    SUBCASE("Devuelve un vector vacio si no hay coincidencias") {
        Catalog c;
        Resource r("bk01", 2000, "Libro");
        r.addTag("cpp");
        c.add(r);

        std::vector<Resource> res = c.findByTag("inexistente");
        CHECK(res.empty());
    }
}
