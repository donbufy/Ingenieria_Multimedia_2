#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>

class Counter {
public:
    Counter();
    Counter(int value);

    int getValue() const;

    void increment();
    bool decrement();
    void reset();
    friend std::ostream& operator<<(std::ostream& os, const Counter& c);

private:
    int value;
};

#endif // COUNTER_H
