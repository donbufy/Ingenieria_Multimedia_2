#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/doctest.h"
#include "Counter.h"

TEST_CASE("A05: Decrementar en cero") {
    Counter c(2);

    // Decrementamos más veces de las que se puede: al llegar a 0 el contador
    // debe quedarse en 0 y decrement() debe devolver false.
    for (int i = 1; i <= 4; ++i) {
        INFO("decremento número ", i);
        int antes = c.getValue();
        CAPTURE(antes);

        bool sePudo = c.decrement();
        CHECK(sePudo == (antes > 0));
    }

    CHECK(c.getValue() == 0);
}
