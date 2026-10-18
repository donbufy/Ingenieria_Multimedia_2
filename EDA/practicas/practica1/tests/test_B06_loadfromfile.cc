#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Catalog.h"

TEST_CASE("B6: loadFromFile") {
    SUBCASE("Carga un fichero valido con el numero correcto de recursos") {
        Catalog c;
        CHECK(c.loadFromFile("data/sample_ok.txt") == true);
        CHECK(c.size() == 6);
    }

    SUBCASE("Ignora lineas invalidas") {
        Catalog c;
        c.loadFromFile("data/sample_mixed.txt");
        CHECK(c.size() == 3);
    }

    SUBCASE("Limpia los datos previos antes de cargar") {
        Catalog c;
        c.add(Resource("viejo", 1990, "Dato previo"));
        CHECK(c.size() == 1);

        c.loadFromFile("data/sample_ok.txt");
        CHECK(c.size() == 6);
        CHECK(c.findById("viejo").getId() == "");
    }

    SUBCASE("Fichero inexistente devuelve false") {
        Catalog c;
        CHECK(c.loadFromFile("no_existe.txt") == false);
    }

    SUBCASE("Fichero vacio carga 0 recursos") {
        Catalog c;
        CHECK(c.loadFromFile("data/sample_empty.txt") == true);
        CHECK(c.size() == 0);
    }
}
