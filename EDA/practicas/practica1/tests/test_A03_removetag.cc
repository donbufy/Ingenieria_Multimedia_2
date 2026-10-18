#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Resource.h"

TEST_CASE("A3: removeTag y hasTag") {
    SUBCASE("removeTag elimina existente y devuelve false si no existe") {
        Resource r("bk01", 2000, "Libro");
        r.addTag("cpp");
        r.addTag("java");

        CHECK(r.removeTag("cpp") == true);
        CHECK(r.hasTag("cpp") == false);
        CHECK(r.hasTag("java") == true);
        CHECK(r.removeTag("inexistente") == false);
    }

    SUBCASE("hasTag normaliza la entrada a minusculas") {
        Resource r("bk01", 2000, "Libro");
        r.addTag("cpp");
        CHECK(r.hasTag("CPP") == true);
        CHECK(r.hasTag("Cpp") == true);
        CHECK(r.hasTag("java") == false);
    }

    SUBCASE("removeTag y hasTag solo normalizan a minusculas, no recortan espacios") {
        // A diferencia de addTag, no deben eliminar espacios al principio
        // o al final: una etiqueta con espacios no debe encontrar la
        // etiqueta ya recortada por addTag.
        Resource r("bk01", 2000, "Libro");
        r.addTag("cpp");
        CHECK(r.hasTag(" cpp") == false);
        CHECK(r.hasTag("cpp ") == false);
        CHECK(r.removeTag(" cpp") == false);
        CHECK(r.hasTag("cpp") == true);
    }
}
