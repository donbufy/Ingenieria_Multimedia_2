#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Playlist.h"

TEST_CASE("B02: insertAt") {
    SUBCASE("Inserta en una lista vacía y la nueva pista pasa a ser la actual") {
        Playlist p;
        CHECK(p.insertAt(0, Track("t01", "Uno", 100)));
        CHECK(p.size() == 1);
        REQUIRE(p.getCurrent() != nullptr);
        CHECK(p.getCurrent()->getId() == "t01");
    }

    SUBCASE("Inserta al principio, en medio y al final") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t03", "Tres", 300));

        CHECK(p.insertAt(1, Track("t02", "Dos", 200)));   // en medio
        CHECK(p.insertAt(0, Track("t00", "Cero", 50)));   // al principio
        CHECK(p.insertAt(p.size(), Track("t04", "Cuatro", 400)));   // al final

        REQUIRE(p.size() == 5);
        CHECK(p.get(0)->getId() == "t00");
        CHECK(p.get(1)->getId() == "t01");
        CHECK(p.get(2)->getId() == "t02");
        CHECK(p.get(3)->getId() == "t03");
        CHECK(p.get(4)->getId() == "t04");
    }

    SUBCASE("Rechaza posiciones fuera de rango") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        CHECK_FALSE(p.insertAt(-1, Track("t02", "Dos", 200)));
        CHECK_FALSE(p.insertAt(2, Track("t02", "Dos", 200)));
        CHECK(p.size() == 1);
    }

    SUBCASE("Rechaza un id duplicado sin modificar la lista") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        CHECK_FALSE(p.insertAt(0, Track("t01", "Otro", 999)));
        CHECK(p.size() == 1);
        CHECK(p.get(0)->getTitle() == "Uno");
    }
}
