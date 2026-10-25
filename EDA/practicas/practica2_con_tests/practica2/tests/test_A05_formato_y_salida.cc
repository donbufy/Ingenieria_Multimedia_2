#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Track.h"
#include <sstream>

TEST_CASE("A05: formatDuration y operator<<") {
    SUBCASE("formatDuration") {
        CHECK(Track("a", "A", 0).formatDuration() == "0:00");
        CHECK(Track("b", "B", 5).formatDuration() == "0:05");
        CHECK(Track("c", "C", 187).formatDuration() == "3:07");
        CHECK(Track("d", "D", 3900).formatDuration() == "65:00");
        CHECK(Track("e", "E", 59).formatDuration() == "0:59");
        CHECK(Track("f", "F", 60).formatDuration() == "1:00");
    }

    SUBCASE("operator<< con el formato exacto") {
        Track t("t01", "Bohemian Rhapsody", 354);
        std::ostringstream oss;
        oss << t;
        CHECK(oss.str() == "t01 (5:54) Bohemian Rhapsody");
    }

    SUBCASE("operator<< con duración corta y título de una palabra") {
        Track t("t02", "Uno", 5);
        std::ostringstream oss;
        oss << t;
        CHECK(oss.str() == "t02 (0:05) Uno");
    }
}
