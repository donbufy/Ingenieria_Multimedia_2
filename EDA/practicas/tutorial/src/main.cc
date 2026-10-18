#include "Counter.h"
#include <iostream>

int main() {
    Counter c;
    c.increment();
    c.increment();
    c.increment();
    std::cout << c << std::endl;
    return 0;
}
