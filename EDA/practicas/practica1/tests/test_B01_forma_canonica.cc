#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"

TEST_CASE("B1: Forma canonica del catalogo") {
    SUBCASE("Constructor por defecto") {
        Catalog c;
        CHECK(c.size() == 0);
    }

    SUBCASE("Copia y asignacion son independientes") {
        Catalog c1;
        Resource r("bk01", 2000, "Libro");
        r.addTag("cpp");
        c1.add(r);

        Catalog c2(c1);
        Catalog c3;
        c3 = c1;

        // Anadir en el original tras copiar no debe afectar a las copias.
        c1.add(Resource("bk02", 2001, "Otro"));
        CHECK(c1.size() == 2);
        CHECK(c2.size() == 1);
        CHECK(c3.size() == 1);

        // Eliminar en una copia no debe afectar al original.
        c2.removeById("bk01");
        CHECK(c2.size() == 0);
        CHECK(c1.size() == 2);
        CHECK(c3.findById("bk01").getId() == "bk01");
    }
}
