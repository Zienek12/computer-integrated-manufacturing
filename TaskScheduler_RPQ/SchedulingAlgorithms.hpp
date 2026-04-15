#pragma once

#include <utility>
#include <vector>

#include "Task.hpp"

int CMax(const std::vector<Zadanie>& zadania);
void sortR(std::vector<Zadanie>& zadania);
void sortRQ(std::vector<Zadanie>& zadania);
int schrage(const std::vector<Zadanie>& zadania);
std::pair<int, std::vector<Zadanie> > schrageKolejnosc(const std::vector<Zadanie>& zadania);
int schragePodzial(const std::vector<Zadanie>& zadania);
