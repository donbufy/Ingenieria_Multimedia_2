#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("C3: DEL") {
    SUBCASE("Elimina un recurso existente y GET posterior responde NOTFOUND") {
        escribirFichero("tests/output/tmp_C3_in.txt",
            "ADD lib01 2024 Libro -\n"
            "DEL lib01\n"
            "GET lib01\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C3_in.txt > tests/output/tmp_C3_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C3_out.txt");
        CHECK(mismaSalida(salida,
            "OK\n"
            "OK\n"
            "NOTFOUND\n"));
    }

    SUBCASE("DEL de un id inexistente responde NOTFOUND") {
        escribirFichero("tests/output/tmp_C3b_in.txt",
            "DEL no_existe\n"
            "EXIT\n");

        int rc = std::system("./catalog < tests/output/tmp_C3b_in.txt > tests/output/tmp_C3b_out.txt 2>&1");
        CHECK(rc == 0);

        std::string salida = leerFichero("tests/output/tmp_C3b_out.txt");
        CHECK(mismaSalida(salida, "NOTFOUND\n"));
    }
}
