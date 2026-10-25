#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C03: PLAY, NEXT y PREV") {
    SUBCASE("PLAY sin pistas responde EMPTY") {
        escribirFichero("tests/output/tmp_C03a_in.txt",
            "PLAY\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C03a_in.txt > tests/output/tmp_C03a_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C03a_out.txt");
        CHECK(mismaSalida(salida, "EMPTY\n"));
    }

    SUBCASE("PLAY muestra la pista actual con el formato de Track::operator<<") {
        escribirFichero("tests/output/tmp_C03b_in.txt",
            "ADD t01 100 Uno\n"
            "PLAY\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C03b_in.txt > tests/output/tmp_C03b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C03b_out.txt");
        CHECK(mismaSalida(salida, "OK\nt01 (1:40) Uno\n"));
    }

    SUBCASE("NEXT avanza de forma circular e imprime la playlist completa") {
        escribirFichero("tests/output/tmp_C03c_in.txt",
            "ADD t01 100 Uno\n"
            "ADD t02 200 Dos\n"
            "ADD t03 300 Tres\n"
            "NEXT\n"
            "NEXT\n"
            "NEXT\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C03c_in.txt > tests/output/tmp_C03c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C03c_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "OK\n"
            "OK\n"
            "  1. t01 (1:40) Uno\n"
            "> 2. t02 (3:20) Dos\n"
            "  3. t03 (5:00) Tres\n"
            "  1. t01 (1:40) Uno\n"
            "  2. t02 (3:20) Dos\n"
            "> 3. t03 (5:00) Tres\n"
            "> 1. t01 (1:40) Uno\n"
            "  2. t02 (3:20) Dos\n"
            "  3. t03 (5:00) Tres\n"));
    }

    SUBCASE("PREV retrocede de forma circular") {
        escribirFichero("tests/output/tmp_C03d_in.txt",
            "ADD t01 100 Uno\n"
            "ADD t02 200 Dos\n"
            "PREV\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C03d_in.txt > tests/output/tmp_C03d_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C03d_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "OK\n"
            "  1. t01 (1:40) Uno\n"
            "> 2. t02 (3:20) Dos\n"));
    }

    SUBCASE("NEXT y PREV en una playlist vacía responden EMPTY") {
        escribirFichero("tests/output/tmp_C03e_in.txt",
            "NEXT\n"
            "PREV\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C03e_in.txt > tests/output/tmp_C03e_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C03e_out.txt");
        CHECK(mismaSalida(salida, "EMPTY\nEMPTY\n"));
    }
}
