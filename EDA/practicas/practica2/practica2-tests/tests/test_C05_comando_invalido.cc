#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C05: comandos inválidos y argumentos sobrantes") {
    SUBCASE("Comando desconocido o con argumentos insuficientes/inválidos responde INVALIDCMD") {
        // FOO -> comando desconocido
        // ADD t01 -> faltan argumentos
        // ADD t01 abc Titulo -> duración no es un entero válido
        // DEL -> falta el id
        // LOAD -> falta el fichero
        escribirFichero("tests/output/tmp_C05a_in.txt",
            "FOO\n"
            "ADD t01\n"
            "ADD t01 abc Titulo\n"
            "DEL\n"
            "LOAD\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C05a_in.txt > tests/output/tmp_C05a_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C05a_out.txt");
        CHECK(mismaSalida(salida,
            "INVALIDCMD\n"
            "INVALIDCMD\n"
            "INVALIDCMD\n"
            "INVALIDCMD\n"
            "INVALIDCMD\n"));
    }

    SUBCASE("Los argumentos sobrantes se ignoran en lugar de producir un error") {
        escribirFichero("tests/output/tmp_C05b_in.txt",
            "ADD t01 100 Uno sobrante\n"
            "LIST extra basura\n"
            "TOTAL algo\n"
            "EXIT\n");

        int rc = std::system("./player < tests/output/tmp_C05b_in.txt > tests/output/tmp_C05b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C05b_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "SIZE 1\n"
            "> 1. t01 (1:40) Uno\n"
            "100\n"));
    }
}
