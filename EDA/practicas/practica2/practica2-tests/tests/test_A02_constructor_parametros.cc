#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Track.h"

TEST_CASE("A02: Constructor con parámetros") {
    Track t("t01", "Bohemian Rhapsody", 354);
    CHECK(t.getId() == "t01");
    CHECK(t.getTitle() == "Bohemian Rhapsody");
    CHECK(t.getDuration() == 354);

    // duración negativa -> 0
    Track t2("t02", "Rara", -10);
    CHECK(t2.getDuration() == 0);
}
