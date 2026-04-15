#pragma once

#include <iosfwd>

struct Zadanie {
    int id;
    int r;
    int p;
    int q;
};

std::ostream& operator<<(std::ostream& os, const Zadanie& zadanie);
