#include "Counter.h"

Counter::Counter() : value(0) {}

Counter::Counter(int value) : value(value) {}

int Counter::getValue() const { return value; }

void Counter::increment() {
    value++;
}

bool Counter::decrement() {
    // BUG: no comprueba si value es 0
    if (value > 0) {value--;
    return true;}
    else return false;
}

std::ostream& operator<<(std::ostream& os, const Counter& c) {
    // BUG: formato incorrecto, debería ser "Counter: <value>"
    os << "Counter: " << c.value;
    return os;
}
void Counter::reset() {
    
        value = 0;

    }
