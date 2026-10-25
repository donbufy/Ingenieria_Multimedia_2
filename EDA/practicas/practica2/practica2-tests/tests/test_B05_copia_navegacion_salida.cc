#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Playlist.h"
#include <sstream>

TEST_CASE("B05: copia profunda, navegación circular, duración total y salida") {
    SUBCASE("El operador de asignación hace una copia profunda e independiente") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));
        p.next();   // current = t02

        Playlist copia;
        copia.add(Track("viejo", "Se debe liberar", 1));
        copia = p;

        CHECK(copia.size() == 2);
        REQUIRE(copia.getCurrent() != nullptr);
        CHECK(copia.getCurrent()->getId() == "t02");
        CHECK(copia.findById("viejo") == nullptr);

        p.removeById("t01");
        CHECK(copia.size() == 2);
    }

    SUBCASE("La autoasignación no corrompe la lista") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));

        p = p;
        CHECK(p.size() == 2);
        CHECK(p.get(0)->getId() == "t01");
        CHECK(p.get(1)->getId() == "t02");
    }

    SUBCASE("next y prev avanzan de forma circular en ambos sentidos") {
        Playlist p;
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 200));
        p.add(Track("t03", "Tres", 300));

        CHECK(p.getCurrent()->getId() == "t01");
        CHECK(p.next());
        CHECK(p.getCurrent()->getId() == "t02");
        CHECK(p.next());
        CHECK(p.getCurrent()->getId() == "t03");
        CHECK(p.next());   // circular: vuelve a la primera
        CHECK(p.getCurrent()->getId() == "t01");

        CHECK(p.prev());   // circular: retrocede a la última
        CHECK(p.getCurrent()->getId() == "t03");
        CHECK(p.prev());
        CHECK(p.getCurrent()->getId() == "t02");
    }

    SUBCASE("next y prev devuelven false en una lista vacía") {
        Playlist p;
        CHECK_FALSE(p.next());
        CHECK_FALSE(p.prev());
    }

    SUBCASE("totalDuration suma la duración de todas las pistas") {
        Playlist p;
        CHECK(p.totalDuration() == 0);
        p.add(Track("t01", "Uno", 100));
        p.add(Track("t02", "Dos", 250));
        p.add(Track("t03", "Tres", 30));
        CHECK(p.totalDuration() == 380);
    }

    SUBCASE("operator<< con el formato exacto, marcador y numeración desde 1") {
        Playlist p;
        p.add(Track("t01", "Uno", 65));
        p.add(Track("t02", "Dos", 200));
        p.next();   // current = t02

        std::ostringstream oss;
        oss << p;
        CHECK(oss.str() ==
            "  1. t01 (1:05) Uno\n"
            "> 2. t02 (3:20) Dos\n");
    }

    SUBCASE("operator<< de una lista vacía no imprime nada") {
        Playlist p;
        std::ostringstream oss;
        oss << p;
        CHECK(oss.str() == "");
    }
}
