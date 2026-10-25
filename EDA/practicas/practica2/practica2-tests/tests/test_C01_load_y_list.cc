#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C01: LOAD y LIST") {
    SUBCASE("LOAD carga el fichero y LIST muestra la playlist con el marcador de la actual") {
        escribirFichero("tests/output/tmp_C01a_in.txt",
            "LOAD data/playlist_ok.txt\n"
            "LIST\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C01a_in.txt > tests/output/tmp_C01a_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C01a_out.txt");
        CHECK(mismaSalida(salida,
            "OK 6\n"
            "SIZE 6\n"
            "> 1. t01 (5:54) Bohemian Rhapsody\n"
            "  2. t02 (8:02) Stairway to Heaven\n"
            "  3. t03 (5:01) Hotel California\n"
            "  4. t04 (2:05) Blitzkrieg Bop\n"
            "  5. t05 (9:43) Shine On\n"
            "  6. t06 (3:25) Should I Stay\n"));
    }

    SUBCASE("LOAD de un fichero inexistente responde ERROR y no modifica la playlist") {
        escribirFichero("tests/output/tmp_C01b_in.txt",
            "LOAD data/playlist_ok.txt\n"
            "LOAD no_existe.txt\n"
            "LIST\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C01b_in.txt > tests/output/tmp_C01b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C01b_out.txt");
        CHECK(mismaSalida(salida,
            "OK 6\n"
            "ERROR\n"
            "SIZE 6\n"
            "> 1. t01 (5:54) Bohemian Rhapsody\n"
            "  2. t02 (8:02) Stairway to Heaven\n"
            "  3. t03 (5:01) Hotel California\n"
            "  4. t04 (2:05) Blitzkrieg Bop\n"
            "  5. t05 (9:43) Shine On\n"
            "  6. t06 (3:25) Should I Stay\n"));
    }

    SUBCASE("LOAD sin argumento responde INVALIDCMD") {
        escribirFichero("tests/output/tmp_C01c_in.txt",
            "LOAD\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C01c_in.txt > tests/output/tmp_C01c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C01c_out.txt");
        CHECK(mismaSalida(salida, "INVALIDCMD\n"));
    }

    SUBCASE("LIST de una playlist vacía muestra SIZE 0 y ninguna otra línea") {
        escribirFichero("tests/output/tmp_C01d_in.txt",
            "LIST\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C01d_in.txt > tests/output/tmp_C01d_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C01d_out.txt");
        CHECK(mismaSalida(salida, "SIZE 0\n"));
    }
}
