#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Track.h"

TEST_CASE("A03: constructor de copia y operador de asignación") {
    SUBCASE("Constructor de copia copia todos los atributos") {
        Track t1("t01", "Bohemian Rhapsody", 354);
        Track t2(t1);
        CHECK(t2.getId() == "t01");
        CHECK(t2.getTitle() == "Bohemian Rhapsody");
        CHECK(t2.getDuration() == 354);
    }

    SUBCASE("Operador de asignación copia todos los atributos") {
        Track t1("t01", "Bohemian Rhapsody", 354);
        Track t2;
        t2 = t1;
        CHECK(t2.getId() == "t01");
        CHECK(t2.getTitle() == "Bohemian Rhapsody");
        CHECK(t2.getDuration() == 354);
    }

    SUBCASE("Asignación encadenada y autoasignación") {
        Track t1("t01", "Uno", 100);
        Track t2("t02", "Dos", 200);
        Track t3("t03", "Tres", 300);

        t3 = t2 = t1;
        CHECK(t2.getId() == "t01");
        CHECK(t3.getId() == "t01");

        t1 = t1;
        CHECK(t1.getId() == "t01");
        CHECK(t1.getTitle() == "Uno");
        CHECK(t1.getDuration() == 100);
    }
}
