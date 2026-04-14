#pragma once

#include <chrono>
#include <vector>

#include "Task.hpp"

struct CarlierWynik {
    int cmax;
    std::vector<Zadanie> kolejnosc;
    bool przekroczonyLimit;
    long long liczbaWezlow;
};

class CarlierSolver {
public:
    CarlierWynik rozwiaz(const std::vector<Zadanie>& zadania, long long limitCzasuMs = 0);

private:
    int UB;
    std::vector<Zadanie> najlepszaKolejnosc;
    bool przekroczonyLimit;
    long long liczbaWezlow;
    long long limitMs;
    std::chrono::steady_clock::time_point start;

    bool czyPrzekroczonoLimitCzasu() const;
    static int silneDolneOgraniczenie(const std::vector<Zadanie>& zadania, int idxC, int rK, int pK, int qK);
    static std::vector<int> czasyZakonczenia(const std::vector<Zadanie>& pi);
    static int indeksPoId(const std::vector<Zadanie>& zadania, int id);
    void carlier(std::vector<Zadanie> zadania);
};
