#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

// COMANDO_RARO produce el primer INVALIDCMD;
// GET sin argumento produce el segundo INVALIDCMD
// ADD no tiene suficientes argumentos y produce el tercer INVALIDCMD
TEST_CASE("C5: Comando desconocido o con argumentos faltantes responde INVALIDCMD") {
    escribirFichero("tests/output/tmp_C5_in.txt",
        "COMANDO_RARO\n"
        "GET\n"
        "ADD id01 2026 MyResource\n"
        "EXIT\n");

    int rc = std::system("./catalog < tests/output/tmp_C5_in.txt > tests/output/tmp_C5_out.txt 2>&1");
    CHECK(rc == 0);

    std::string salida = leerFichero("tests/output/tmp_C5_out.txt");
    CHECK(mismaSalida(salida,
        "INVALIDCMD\n"
        "INVALIDCMD\n"
        "INVALIDCMD\n"));
}
