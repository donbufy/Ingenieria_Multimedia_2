#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Playlist.h"

TEST_CASE("B01: Playlist vacía y add") {
    SUBCASE("Una playlist recién creada está vacía") {
        Playlist p;
        CHECK(p.size() == 0);
        CHECK(p.empty());
        CHECK(p.getCurrent() == nullptr);
        CHECK(p.get(0) == nullptr);
        CHECK(p.findById("t01") == nullptr);
        CHECK(p.totalDuration() == 0);
    }

    SUBCASE("add añade al final y la primera pista pasa a ser la actual") {
        Playlist p;
        CHECK(p.add(Track("t01", "Uno", 100)));
        REQUIRE(p.getCurrent() != nullptr);
        CHECK(p.getCurrent()->getId() == "t01");

        CHECK(p.add(Track("t02", "Dos", 200)));
        CHECK(p.add(Track("t03", "Tres", 300)));
        CHECK(p.size() == 3);
        CHECK_FALSE(p.empty());
        // añadir pistas después de la primera no cambia la actual
        CHECK(p.getCurrent()->getId() == "t01");

        REQUIRE(p.get(0) != nullptr);
        CHECK(p.get(0)->getId() == "t01");
        CHECK(p.get(1)->getId() == "t02");
        CHECK(p.get(2)->getId() == "t03");
        CHECK(p.get(3) == nullptr);
        CHECK(p.get(-1) == nullptr);
    }

    SUBCASE("findById encuentra o devuelve nullptr") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));

        REQUIRE(p.findById("t02") != nullptr);
        CHECK(p.findById("t02")->getTitle() == "Dos");
        CHECK(p.findById("inexistente") == nullptr);
    }
}
