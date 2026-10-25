#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C04: TOTAL y EXIT") {
    SUBCASE("TOTAL de una playlist vacía es 0") {
        escribirFichero("tests/output/tmp_C04a_in.txt",
            "TOTAL\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C04a_in.txt > tests/output/tmp_C04a_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C04a_out.txt");
        CHECK(mismaSalida(salida, "0\n"));
    }

    SUBCASE("TOTAL imprime únicamente la suma de duraciones, sin prefijo") {
        escribirFichero("tests/output/tmp_C04b_in.txt",
            "ADD t01 100 Uno\n"
            "ADD t02 250 Dos\n"
            "TOTAL\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C04b_in.txt > tests/output/tmp_C04b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C04b_out.txt");
        CHECK(mismaSalida(salida, "OK\nOK\n350\n"));
    }

    SUBCASE("EXIT termina el programa sin imprimir nada y sin procesar lo que sigue") {
        escribirFichero("tests/output/tmp_C04c_in.txt",
            "EXIT\n"
            "TOTAL\n");

        int rc = std::system("./player < tests/output/tmp_C04c_in.txt > tests/output/tmp_C04c_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C04c_out.txt");
        CHECK(mismaSalida(salida, ""));
    }

    SUBCASE("Las líneas vacías se ignoran sin imprimir nada") {
        escribirFichero("tests/output/tmp_C04d_in.txt",
            "\n"
            "ADD t01 100 Uno\n"
            "\n"
            "TOTAL\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C04d_in.txt > tests/output/tmp_C04d_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C04d_out.txt");
        CHECK(mismaSalida(salida, "OK\n100\n"));
    }
}
