#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C1: LOAD y GET") {
    SUBCASE("LOAD carga fichero valido y GET encuentra el recurso") {
        escribirFichero("tests/output/tmp_C1_in.txt",
            "LOAD data/sample_ok.txt\n"
            "GET bk01\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C1_in.txt > tests/output/tmp_C1_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C1_out.txt");
        CHECK(mismaSalida(salida,
            "OK 6\n"
            "bk01 (1999) Clean Code [craft,software]\n"));
    }

    SUBCASE("LOAD con fichero inexistente responde ERROR") {
        escribirFichero("tests/output/tmp_C1b_in.txt",
            "LOAD no_existe.txt\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C1b_in.txt > tests/output/tmp_C1b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C1b_out.txt");
        CHECK(mismaSalida(salida, "ERROR\n"));
    }

    SUBCASE("GET de un id inexistente responde NOTFOUND") {
        escribirFichero("tests/output/tmp_C1c_in.txt",
            "GET no_existe\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C1c_in.txt > tests/output/tmp_C1c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C1c_out.txt");
        CHECK(mismaSalida(salida, "NOTFOUND\n"));
    }
}
