#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Track.h"

TEST_CASE("A01: Constructor por defecto") {
    Track t;
    CHECK(t.getId() == "");
    CHECK(t.getTitle() == "");
    CHECK(t.getDuration() == 0);
}
