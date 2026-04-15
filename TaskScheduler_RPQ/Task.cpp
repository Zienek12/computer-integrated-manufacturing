#include "Task.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Zadanie& zadanie) {
    os << "Zadanie ID: " << zadanie.id
       << ", r: " << zadanie.r
       << ", p: " << zadanie.p
       << ", q: " << zadanie.q;
    return os;
}
