#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"

TEST_CASE("B4: removeById") {
    SUBCASE("Elimina un recurso existente y actualiza el tamano") {
        Catalog c;
        c.add(Resource("bk01", 2000, "Libro 1"));
        c.add(Resource("bk02", 2001, "Libro 2"));
        CHECK(c.size() == 2);

        CHECK(c.removeById("bk01") == true);
        CHECK(c.size() == 1);
        CHECK(c.findById("bk01").getId() == "");
    }

    SUBCASE("Devuelve false si el id no existe") {
        Catalog c;
        c.add(Resource("bk01", 2000, "Libro"));
        CHECK(c.removeById("inexistente") == false);
        CHECK(c.size() == 1);
    }
}
