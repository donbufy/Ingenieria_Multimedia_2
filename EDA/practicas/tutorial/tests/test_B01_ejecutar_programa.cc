#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "common/test_utils.h"
#include <cstdlib>

TEST_CASE("B01: Ejecutar programa") {
    int rc = std::system("./counter > tests/output/tmp_B1_out.txt 2>&1");
    CHECK(rc == 0);

    std::string salida = leerFichero("tests/output/tmp_B1_out.txt");
    CHECK(salida.find("Counter: 3") != std::string::npos);
}
