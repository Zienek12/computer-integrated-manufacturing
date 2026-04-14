#include "CarlierSolver.hpp"

#include <algorithm>
#include <chrono>
#include <limits>
#include <utility>
#include <vector>

#include "SchedulingAlgorithms.hpp"

bool CarlierSolver::czyPrzekroczonoLimitCzasu() const {
    if (limitMs <= 0) {
        return false;
    }

    const long long uplyneloMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start)
        .count();
    return uplyneloMs >= limitMs;
}

int CarlierSolver::silneDolneOgraniczenie(const std::vector<Zadanie>& zadania, int idxC, int rK, int pK, int qK) {
    const int lbSchragePmtn = schragePodzial(zadania);
    const int hK = rK + pK + qK;
    const int hKc = std::min(zadania[idxC].r, rK) + zadania[idxC].p + pK + std::min(zadania[idxC].q, qK);
    return std::max(lbSchragePmtn, std::max(hK, hKc));
}

std::vector<int> CarlierSolver::czasyZakonczenia(const std::vector<Zadanie>& pi) {
    std::vector<int> C(pi.size(), 0);
    int t = 0;
    for (std::size_t i = 0; i < pi.size(); ++i) {
        t = std::max(t, pi[i].r) + pi[i].p;
        C[i] = t;
    }
    return C;
}

int CarlierSolver::indeksPoId(const std::vector<Zadanie>& zadania, int id) {
    int idx = id - 1;
    if (idx >= 0 && idx < static_cast<int>(zadania.size()) && zadania[idx].id == id) {
        return idx;
    }
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        if (zadania[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void CarlierSolver::carlier(std::vector<Zadanie> zadania) {
    if (przekroczonyLimit) {
        return;
    }
    if (czyPrzekroczonoLimitCzasu()) {
        przekroczonyLimit = true;
        return;
    }
    ++liczbaWezlow;

    const std::pair<int, std::vector<Zadanie> > wynikSchrage = schrageKolejnosc(zadania);
    const int U = wynikSchrage.first;
    const std::vector<Zadanie> pi = wynikSchrage.second;

    if (U < UB) {
        UB = U;
        najlepszaKolejnosc = pi;
    }

    const std::vector<int> C = czasyZakonczenia(pi);

    int b = -1;
    for (int i = static_cast<int>(pi.size()) - 1; i >= 0; --i) {
        if (C[i] + pi[i].q == U) {
            b = i;
            break;
        }
    }
    if (b == -1) {
        return;
    }

    int a = -1;
    int sumaP = 0;
    for (int i = b; i >= 0; --i) {
        sumaP += pi[i].p;
        if (pi[i].r + sumaP + pi[b].q == U) {
            a = i;
        }
    }
    if (a == -1) {
        return;
    }

    int c = -1;
    for (int i = b - 1; i >= a; --i) {
        if (pi[i].q < pi[b].q) {
            c = i;
            break;
        }
    }
    if (c == -1) {
        return;
    }

    int rPrim = std::numeric_limits<int>::max();
    int qPrim = std::numeric_limits<int>::max();
    int pPrim = 0;
    for (int i = c + 1; i <= b; ++i) {
        rPrim = std::min(rPrim, pi[i].r);
        qPrim = std::min(qPrim, pi[i].q);
        pPrim += pi[i].p;
    }

    const int idC = pi[c].id;
    const int noweR = rPrim + pPrim;
    const int noweQ = qPrim + pPrim;

    std::vector<Zadanie> zadania1 = zadania;
    const int idx1 = indeksPoId(zadania1, idC);
    if (idx1 != -1) {
        const int stareR = zadania1[idx1].r;
        zadania1[idx1].r = std::max(zadania1[idx1].r, noweR);
        if (zadania1[idx1].r != stareR) {
            const int LB = silneDolneOgraniczenie(zadania1, idx1, rPrim, pPrim, qPrim);
            if (LB < UB) {
                carlier(zadania1);
            }
        }
    }

    std::vector<Zadanie> zadania2 = zadania;
    const int idx2 = indeksPoId(zadania2, idC);
    if (idx2 != -1) {
        const int stareQ = zadania2[idx2].q;
        zadania2[idx2].q = std::max(zadania2[idx2].q, noweQ);
        if (zadania2[idx2].q != stareQ) {
            const int LB = silneDolneOgraniczenie(zadania2, idx2, rPrim, pPrim, qPrim);
            if (LB < UB) {
                carlier(zadania2);
            }
        }
    }
}

CarlierWynik CarlierSolver::rozwiaz(const std::vector<Zadanie>& zadania, long long limitCzasuMs) {
    UB = std::numeric_limits<int>::max();
    najlepszaKolejnosc.clear();
    przekroczonyLimit = false;
    liczbaWezlow = 0;
    limitMs = limitCzasuMs;
    start = std::chrono::steady_clock::now();

    carlier(zadania);
    return CarlierWynik{UB, najlepszaKolejnosc, przekroczonyLimit, liczbaWezlow};
}
