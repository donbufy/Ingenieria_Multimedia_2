#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C02: ADD y DEL") {
    SUBCASE("ADD con id duplicado responde DUPLICATE") {
        escribirFichero("tests/output/tmp_C02b_in.txt",
            "ADD t01 100 Uno\n"
            "ADD t01 200 Otro\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C02b_in.txt > tests/output/tmp_C02b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C02b_out.txt");
        CHECK(mismaSalida(salida, "OK\nDUPLICATE\n"));
    }

    SUBCASE("ADD con argumentos inválidos o insuficientes responde INVALIDCMD") {
        escribirFichero("tests/output/tmp_C02c_in.txt",
            "ADD t01 -5 Uno\n"
            "ADD t02 abc Dos\n"
            "ADD t03\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C02c_in.txt > tests/output/tmp_C02c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C02c_out.txt");
        CHECK(mismaSalida(salida, "INVALIDCMD\nINVALIDCMD\nINVALIDCMD\n"));
    }

    SUBCASE("DEL elimina una pista existente y actualiza la actual") {
        escribirFichero("tests/output/tmp_C02d_in.txt",
            "ADD t01 100 Uno\n"
            "ADD t02 200 Dos\n"
            "DEL t01\n"
            "LIST\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C02d_in.txt > tests/output/tmp_C02d_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C02d_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "OK\n"
            "OK\n"
            "SIZE 1\n"
            "> 1. t02 (3:20) Dos\n"));
    }

    SUBCASE("DEL de un id inexistente responde NOTFOUND; sin argumento responde INVALIDCMD") {
        escribirFichero("tests/output/tmp_C02e_in.txt",
            "DEL no_existe\n"
            "DEL\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C02e_in.txt > tests/output/tmp_C02e_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C02e_out.txt");
        CHECK(mismaSalida(salida, "NOTFOUND\nINVALIDCMD\n"));
    }
}
