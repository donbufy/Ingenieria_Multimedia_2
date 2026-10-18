#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Resource.h"
#include <vector>
#include <string>

TEST_CASE("A2: addTag") {
    SUBCASE("Anade etiquetas nuevas y mantiene orden lexicografico") {
        Resource r("bk01", 2000, "Libro");
        CHECK(r.addTag("zebra") == true);
        CHECK(r.addTag("alpha") == true);
        CHECK(r.addTag("middle") == true);

        std::vector<std::string> tags = r.getTags();
        REQUIRE(tags.size() == 3);
        CHECK(tags[0] == "alpha");
        CHECK(tags[1] == "middle");
        CHECK(tags[2] == "zebra");
    }

    SUBCASE("Rechaza etiquetas vacias o solo espacios") {
        Resource r("bk01", 2000, "Libro");
        CHECK(r.addTag("") == false);
        CHECK(r.addTag("   ") == false);
        CHECK(r.getTags().empty());
    }

    SUBCASE("Recorta espacios al principio y al final") {
        Resource r("bk01", 2000, "Libro");
        CHECK(r.addTag("  craft  ") == true);
        CHECK(r.hasTag("craft") == true);
        // Misma etiqueta con espacios: se detecta como duplicado tras recortar.
        CHECK(r.addTag("  craft ") == false);
        CHECK(r.getTags().size() == 1);
    }

    SUBCASE("Convierte la etiqueta a minusculas") {
        Resource r("bk01", 2000, "Libro");
        CHECK(r.addTag("CRAFT") == true);
        CHECK(r.hasTag("craft") == true);
        // Misma etiqueta en mayusculas: se detecta como duplicado tras normalizar.
        CHECK(r.addTag("CRAFT") == false);
        CHECK(r.getTags().size() == 1);
    }

    SUBCASE("Constructor copia y operador asignacion copian tags") {
        Resource r1("bk01", 1999, "Clean Code");
        r1.addTag("cpp");

        Resource r2(r1);
        Resource r3;
        r3 = r1;

        CHECK(r2.getId() == "bk01");
        CHECK(r2.hasTag("cpp"));
        CHECK(r3.getId() == "bk01");
        CHECK(r3.hasTag("cpp"));

        // Modificar el original no debe afectar a las copias.
        r1.addTag("programming");
        CHECK(r1.hasTag("programming"));
        CHECK_FALSE(r2.hasTag("programming"));
        CHECK_FALSE(r3.hasTag("programming"));
    }
}
