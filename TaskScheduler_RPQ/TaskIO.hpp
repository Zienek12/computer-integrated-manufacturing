#pragma once

#include <string>
#include <vector>

#include "Task.hpp"

std::vector<Zadanie> wczytajZadania(const std::string& nazwaPliku);
void printZadania(const std::vector<Zadanie>& zadania);
