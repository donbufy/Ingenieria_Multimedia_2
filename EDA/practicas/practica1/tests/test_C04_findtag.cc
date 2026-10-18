#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C4: FINDTAG") {
    SUBCASE("Muestra la cuenta y los recursos con esa etiqueta") {
        escribirFichero("tests/output/tmp_C4_in.txt",
            "ADD bk01 2000 Libro_Uno cpp\n"
            "ADD bk02 2001 Libro_Dos cpp,java\n"
            "FINDTAG cpp\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C4_in.txt > tests/output/tmp_C4_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C4_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "OK\n"
            "COUNT 2\n"
            "bk01 (2000) Libro Uno [cpp]\n"
            "bk02 (2001) Libro Dos [cpp,java]\n"));
    }

    SUBCASE("Sin coincidencias responde COUNT 0") {
        escribirFichero("tests/output/tmp_C4b_in.txt",
            "ADD bk01 2000 Libro_Uno cpp\n"
            "FINDTAG inexistente\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C4b_in.txt > tests/output/tmp_C4b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C4b_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "COUNT 0\n"));
    }
}
