#include "TaskIO.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

std::vector<Zadanie> wczytajZadania(const std::string& nazwaPliku) {
    std::vector<Zadanie> zadania;
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku: " << nazwaPliku << std::endl;
        return zadania;
    }

    int n;
    std::string firstToken;
    plik >> firstToken;
    try {
        n = std::stoi(firstToken);
    } catch (const std::invalid_argument&) {
        plik >> n;
    }

    for (int i = 0; i < n; ++i) {
        Zadanie zadanie;
        zadanie.id = i + 1;
        plik >> zadanie.r >> zadanie.p >> zadanie.q;
        zadania.push_back(zadanie);
    }

    return zadania;
}

void printZadania(const std::vector<Zadanie>& zadania) {
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        std::cout << zadania[i] << std::endl;
    }
}
