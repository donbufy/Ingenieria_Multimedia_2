#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Playlist.h"

TEST_CASE("B04: load, merge y clear") {
    SUBCASE("load de un fichero válido carga todas las pistas y fija la actual") {
        Playlist p;
        CHECK(p.load("data/playlist_ok.txt"));
        REQUIRE(p.size() == 6);
        REQUIRE(p.getCurrent() != nullptr);
        CHECK(p.getCurrent()->getId() == "t01");
        CHECK(p.get(5)->getId() == "t06");
    }

    SUBCASE("load de un fichero inexistente devuelve false y no modifica la lista") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        CHECK_FALSE(p.load("no_existe.txt"));
        CHECK(p.size() == 1);
        CHECK(p.get(0)->getId() == "t01");
    }

    SUBCASE("load ignora las líneas inválidas y los ids repetidos") {
        Playlist p;
        CHECK(p.load("data/playlist_mixed.txt"));
        CHECK(p.size() == 3);
        CHECK(p.get(0)->getId() == "t01");
        CHECK(p.get(1)->getId() == "t04");
        CHECK(p.get(2)->getId() == "t06");
    }

    SUBCASE("load de un fichero vacío deja la lista vacía") {
        Playlist p;
        CHECK(p.load("data/playlist_empty.txt"));
        CHECK(p.size() == 0);
        CHECK(p.getCurrent() == nullptr);
    }

    SUBCASE("merge añade solo las pistas con id nuevo y no modifica other") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));

        Playlist other;
        other.add(Track("t02", "Dos (otra copia)", 999));   // ya existe en p
        other.add(Track("t03", "Tres", 300));

        CHECK(p.merge(other) == 1);
        REQUIRE(p.size() == 3);
        CHECK(p.get(2)->getId() == "t03");
        CHECK(p.get(1)->getTitle() == "Dos");   // no se sobrescribe

        // other no se modifica
        CHECK(other.size() == 2);
    }

    SUBCASE("clear deja la lista vacía y reutilizable") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));

        p.clear();
        CHECK(p.size() == 0);
        CHECK(p.empty());
        CHECK(p.getCurrent() == nullptr);

        CHECK(p.add(Track("t03", "Tres", 300)));
        CHECK(p.size() == 1);
    }
}
