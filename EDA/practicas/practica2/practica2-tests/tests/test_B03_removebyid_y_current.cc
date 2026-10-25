#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Playlist.h"

TEST_CASE("B03: removeById y gestión de current") {
    SUBCASE("Elimina un nodo intermedio, la cabeza y el último manteniendo el encadenado") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));
        p.add(Track("t03", "Tres", 300));

        CHECK(p.removeById("t02"));       // nodo intermedio
        REQUIRE(p.size() == 2);
        CHECK(p.get(0)->getId() == "t01");
        CHECK(p.get(1)->getId() == "t03");

        CHECK(p.removeById("t01"));       // cabeza
        CHECK(p.get(0)->getId() == "t03");

        CHECK(p.removeById("t03"));       // último
        CHECK(p.size() == 0);
        CHECK(p.get(0) == nullptr);
    }

    SUBCASE("removeById de un id inexistente devuelve false y no modifica la lista") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        CHECK_FALSE(p.removeById("inexistente"));
        CHECK(p.size() == 1);
    }

    SUBCASE("Al eliminar la pista actual, la actual pasa a ser la siguiente") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));
        p.add(Track("t03", "Tres", 300));
        // current == t01

        CHECK(p.removeById("t01"));
        REQUIRE(p.getCurrent() != nullptr);
        CHECK(p.getCurrent()->getId() == "t02");
    }

    SUBCASE("Al eliminar la única pista, la lista queda sin pista actual") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        CHECK(p.removeById("t01"));
        CHECK(p.empty());
        CHECK(p.getCurrent() == nullptr);
    }
}
