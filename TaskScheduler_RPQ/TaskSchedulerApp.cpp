#include "TaskSchedulerApp.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "Benchmark.hpp"
#include "CarlierSolver.hpp"
#include "SchedulingAlgorithms.hpp"
#include "TaskIO.hpp"

namespace {

std::string zbudujSciezkeDanych(const std::string& nazwaPliku) {
    const std::string path = "C:\\Users\\andos\\source\\repos\\Zienek12\\computer-integrated-manufacturing\\TaskScheduler_RPQ";
    return path + "\\" + nazwaPliku;
}

void uruchomDlaPliku(const std::string& plik) {
    std::cout << "------ " << plik << " ------" << std::endl;
    const std::vector<Zadanie> zadania = wczytajZadania(zbudujSciezkeDanych(plik));

    const std::pair<int, long long> wynikCzas1 = zmierzCzas([&]() { return CMax(zadania); });
    std::cout << "Wynik dla sortowania 123..: " << wynikCzas1.first << " (czas: " << wynikCzas1.second << " us)" << std::endl;

    std::vector<Zadanie> zadaniaR = zadania;
    const std::pair<int, long long> wynikCzas2 = zmierzCzas([&]() {
        sortR(zadaniaR);
        return CMax(zadaniaR);
    });
    std::cout << "Wynik po sortowaniu po R: " << wynikCzas2.first << " (czas: " << wynikCzas2.second << " us)" << std::endl;

    std::vector<Zadanie> zadaniaRQ = zadania;
    const std::pair<int, long long> wynikCzas3 = zmierzCzas([&]() {
        sortRQ(zadaniaRQ);
        return CMax(zadaniaRQ);
    });
    std::cout << "Wynik po sortowaniu po R i Q: " << wynikCzas3.first << " (czas: " << wynikCzas3.second << " us)" << std::endl;

    const std::pair<int, long long> wynikCzasSchrage = zmierzCzas([&]() { return schrage(zadania); });
    std::cout << "Wynik algorytmu Schrage: " << wynikCzasSchrage.first << " (czas: " << wynikCzasSchrage.second << " us)" << std::endl;

    const std::pair<CarlierWynik, long long> wynikCzasCarlier = zmierzCzas([&]() {
        CarlierSolver solver;
        const long long limitDlaInstancjiMs = (plik == "data4.txt") ? 10000 : 0;
        return solver.rozwiaz(zadania, limitDlaInstancjiMs);
    });

    std::cout << "Wynik algorytmu Carliera: " << wynikCzasCarlier.first.cmax
              << " (czas: " << wynikCzasCarlier.second << " us, wezly: " << wynikCzasCarlier.first.liczbaWezlow << ")";
    if (wynikCzasCarlier.first.przekroczonyLimit) {
        std::cout << " [przekroczono limit czasu - wynik moze nie byc optymalny]";
    }
    std::cout << std::endl;
}

}  // namespace

void TaskSchedulerApp::run() const {
    const std::vector<std::string> pliki = {"data1.txt", "data2.txt", "data3.txt", "data4.txt"};
    for (std::size_t i = 0; i < pliki.size(); ++i) {
        uruchomDlaPliku(pliki[i]);
    }
}
