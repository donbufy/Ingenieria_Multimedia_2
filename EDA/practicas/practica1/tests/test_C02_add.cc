#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C2: ADD") {
    SUBCASE("Anade recurso (titulo con guiones bajos y tags) y GET lo muestra") {
        escribirFichero("tests/output/tmp_C2_in.txt",
            "ADD lib01 2024 Mi_Libro cpp,java\n"
            "GET lib01\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C2_in.txt > tests/output/tmp_C2_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C2_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "lib01 (2024) Mi Libro [cpp,java]\n"));
    }

    SUBCASE("ADD sin tags usando '-' anade el recurso sin etiquetas") {
        escribirFichero("tests/output/tmp_C2b_in.txt",
            "ADD lib02 2024 Otro_Libro -\n"
            "GET lib02\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C2b_in.txt > tests/output/tmp_C2b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C2b_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "lib02 (2024) Otro Libro []\n"));
    }

    SUBCASE("ADD duplicado responde DUPLICATE") {
        escribirFichero("tests/output/tmp_C2c_in.txt",
            "ADD bk01 2000 Libro cpp\n"
            "ADD bk01 2001 Otro_Libro java\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C2c_in.txt > tests/output/tmp_C2c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C2c_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "DUPLICATE\n"));
    }
}
