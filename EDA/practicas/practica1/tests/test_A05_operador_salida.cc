#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Resource.h"
#include <sstream>

TEST_CASE("A5: operator<<") {
    SUBCASE("Formato con tags") {
        Resource r("bk01", 1999, "Clean Code");
        r.addTag("software");
        r.addTag("craft");
        std::ostringstream oss;
        oss << r;
        CHECK(oss.str() == "bk01 (1999) Clean Code [craft,software]");
    }

    SUBCASE("Formato sin tags") {
        Resource r("bk02", 2020, "Sin Etiquetas");
        std::ostringstream oss;
        oss << r;
        CHECK(oss.str() == "bk02 (2020) Sin Etiquetas []");
    }
}
