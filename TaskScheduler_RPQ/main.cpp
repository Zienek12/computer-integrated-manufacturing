#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <queue>
#include <limits>

using namespace std;

struct Zadanie {
    int id;
    int r; 
    int p; 
    int q;
};
ostream& operator<<(ostream& os, const Zadanie& zadanie) {
    os << "Zadanie ID: " << zadanie.id
        << ", r: " << zadanie.r
        << ", p: " << zadanie.p
        << ", q: " << zadanie.q;
    return os;
}



vector<Zadanie> wczytajZadania(const string& nazwaPliku) {
    vector<Zadanie> zadania;
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << nazwaPliku << endl;
        return zadania;
    }

    int n;
    string firstToken;
    plik >> firstToken;
    try {
        n = stoi(firstToken);
    } catch (const invalid_argument&) {
        plik >> n;
    }

    for (int i = 0; i < n; ++i) {
        Zadanie zadanie;
        zadanie.id = i + 1;
        plik >> zadanie.r >> zadanie.p >> zadanie.q;
        zadania.push_back(zadanie);
    }
    plik.close();
    return zadania;
}

void printZadania(const vector<Zadanie>& zadania) {
    for (const auto& zadanie : zadania) {
        cout << zadanie << endl;
    }
}

int CMax(const vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;
    for (const auto& zadanie : zadania) {
        t = max(t, zadanie.r) + zadanie.p;
		cmax = max(cmax, t + zadanie.q);
    
    }

    return cmax; 
}

void sortR(vector<Zadanie>& zadania) {
    sort(zadania.begin(), zadania.end(), [](const Zadanie& a, const Zadanie& b) {
        return a.r < b.r;
        });
}

void sortRQ(vector<Zadanie>& zadania) {
    sort(zadania.begin(), zadania.end(), [](const Zadanie& a, const Zadanie& b) {
        // Jesli r sa takie same zadanie z wiekszym q ma priorytet
        if (a.r == b.r) {
            return a.q > b.q;
        }
        return a.r < b.r;
        });
}

struct CompareQ {
    bool operator()(const Zadanie& a, const Zadanie& b) {
        return a.q < b.q; // Max-heap (największe q na szczycie)
    }
};

struct CompareR {
    bool operator()(const Zadanie& a, const Zadanie& b) {
        return a.r > b.r; // Min-heap (najmniejsze r na szczycie)
    }
};

int schrage(const vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;

    priority_queue<Zadanie, vector<Zadanie>, CompareR> N;
    for (const auto& z : zadania) {
        N.push(z);
    }

    priority_queue<Zadanie, vector<Zadanie>, CompareQ> G;

    while (!N.empty() || !G.empty()) {
        while (!N.empty() && N.top().r <= t) {
            G.push(N.top());
            N.pop();
        }

        if (G.empty()) {
            t = N.top().r;
            continue;
        }

        Zadanie j = G.top();
        G.pop();
        t += j.p;
        cmax = max(cmax, t + j.q);
    }

    return cmax;
}

pair<int, vector<Zadanie>> schrageKolejnosc(const vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;
    vector<Zadanie> pi;

    priority_queue<Zadanie, vector<Zadanie>, CompareR> N;
    for (const auto& z : zadania) {
        N.push(z);
    }

    priority_queue<Zadanie, vector<Zadanie>, CompareQ> G;

    while (!N.empty() || !G.empty()) {
        while (!N.empty() && N.top().r <= t) {
            G.push(N.top());
            N.pop();
        }

        if (G.empty()) {
            t = N.top().r;
            continue;
        }

        Zadanie j = G.top();
        G.pop();
        pi.push_back(j);
        t += j.p;
        cmax = max(cmax, t + j.q);
    }

    return { cmax, pi };
}

int schragePodzial(const vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;

    priority_queue<Zadanie, vector<Zadanie>, CompareR> N;
    for (const auto& z : zadania) {
        N.push(z);
    }

    priority_queue<Zadanie, vector<Zadanie>, CompareQ> G;
    Zadanie l = { -1, 0, 0, 0 };

    while (!N.empty() || !G.empty()) {
        while (!N.empty() && N.top().r <= t) {
            Zadanie j = N.top();
            N.pop();
            G.push(j);

            if (j.q > l.q) {
                l.p = t - j.r;
                t = j.r;
                if (l.p > 0) {
                    G.push(l);
                }
            }
        }

        if (G.empty()) {
            t = N.top().r;
            continue;
        }

        Zadanie j = G.top();
        G.pop();
        l = j;
        t += j.p;
        cmax = max(cmax, t + j.q);
    }

    return cmax;
}

struct CarlierWynik {
    int cmax;
    vector<Zadanie> kolejnosc;
    bool przekroczonyLimit;
    long long liczbaWezlow;
};

class CarlierSolver {
private:
    int UB = numeric_limits<int>::max();
    vector<Zadanie> najlepszaKolejnosc;
    bool przekroczonyLimit = false;
    long long liczbaWezlow = 0;
    long long limitMs = 0;
    chrono::steady_clock::time_point start;

    bool czyPrzekroczonoLimitCzasu() {
        if (limitMs <= 0) {
            return false;
        }
        long long uplyneloMs = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
        return uplyneloMs >= limitMs;
    }

    static int silneDolneOgraniczenie(const vector<Zadanie>& zadania, int idxC, int rK, int pK, int qK) {
        int lbSchragePmtn = schragePodzial(zadania);
        int hK = rK + pK + qK;
        int hKc = min(zadania[idxC].r, rK) + zadania[idxC].p + pK + min(zadania[idxC].q, qK);
        return max(lbSchragePmtn, max(hK, hKc));
    }

    static vector<int> czasyZakonczenia(const vector<Zadanie>& pi) {
        vector<int> C(pi.size(), 0);
        int t = 0;
        for (size_t i = 0; i < pi.size(); ++i) {
            t = max(t, pi[i].r) + pi[i].p;
            C[i] = t;
        }
        return C;
    }

    static int indeksPoId(const vector<Zadanie>& zadania, int id) {
        int idx = id - 1;
        if (idx >= 0 && idx < static_cast<int>(zadania.size()) && zadania[idx].id == id) {
            return idx;
        }
        for (size_t i = 0; i < zadania.size(); ++i) {
            if (zadania[i].id == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    void carlier(vector<Zadanie> zadania) {
        if (przekroczonyLimit) {
            return;
        }
        if (czyPrzekroczonoLimitCzasu()) {
            przekroczonyLimit = true;
            return;
        }
        ++liczbaWezlow;

        pair<int, vector<Zadanie>> wynikSchrage = schrageKolejnosc(zadania);
        int U = wynikSchrage.first;
        vector<Zadanie> pi = wynikSchrage.second;

        if (U < UB) {
            UB = U;
            najlepszaKolejnosc = pi;
        }

        vector<int> C = czasyZakonczenia(pi);

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

        int rPrim = numeric_limits<int>::max();
        int qPrim = numeric_limits<int>::max();
        int pPrim = 0;
        for (int i = c + 1; i <= b; ++i) {
            rPrim = min(rPrim, pi[i].r);
            qPrim = min(qPrim, pi[i].q);
            pPrim += pi[i].p;
        }

        const int idC = pi[c].id;
        const int noweR = rPrim + pPrim;
        const int noweQ = qPrim + pPrim;

        vector<Zadanie> zadania1 = zadania;
        int idx1 = indeksPoId(zadania1, idC);
        if (idx1 != -1) {
            int stareR = zadania1[idx1].r;
            zadania1[idx1].r = max(zadania1[idx1].r, noweR);
            if (zadania1[idx1].r != stareR) {
                int LB = silneDolneOgraniczenie(zadania1, idx1, rPrim, pPrim, qPrim);
                if (LB < UB) {
                    carlier(zadania1);
                }
            }
        }

        vector<Zadanie> zadania2 = zadania;
        int idx2 = indeksPoId(zadania2, idC);
        if (idx2 != -1) {
            int stareQ = zadania2[idx2].q;
            zadania2[idx2].q = max(zadania2[idx2].q, noweQ);
            if (zadania2[idx2].q != stareQ) {
                int LB = silneDolneOgraniczenie(zadania2, idx2, rPrim, pPrim, qPrim);
                if (LB < UB) {
                    carlier(zadania2);
                }
            }
        }
    }

public:
    CarlierWynik rozwiaz(const vector<Zadanie>& zadania, long long limitCzasuMs = 0) {
        UB = numeric_limits<int>::max();
        najlepszaKolejnosc.clear();
        przekroczonyLimit = false;
        liczbaWezlow = 0;
        limitMs = limitCzasuMs;
        start = chrono::steady_clock::now();

        carlier(zadania);
        return { UB, najlepszaKolejnosc, przekroczonyLimit, liczbaWezlow };
    }
};

template <typename Func>
auto zmierzCzas(Func&& func) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto wynik = func();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    return make_pair(wynik, duration);
}

void wlaczZadania()
{
    const vector<string> pliki = { "data1.txt", "data2.txt", "data3.txt", "data4.txt" };
    const string path = "C:\\Users\\andos\\source\\repos\\Zienek12\\computer-integrated-manufacturing\\TaskScheduler_RPQ";

    for (const auto& plik : pliki) {
        cout << "------ " << plik << " ------" << endl;
        string nazwaPliku = path + "\\" + plik;
        const vector<Zadanie> zadania = wczytajZadania(nazwaPliku);

        // CMax bez sortowania
        auto wynikCzas1 = zmierzCzas([&]() { return CMax(zadania); });
        cout << "Wynik dla sortowania 123..: " << wynikCzas1.first << " (czas: " << wynikCzas1.second << " us)" << endl;

        // CMax po sortowaniu po R
        vector<Zadanie> zadaniaR = zadania;
        auto wynikCzas2 = zmierzCzas([&]() {
            sortR(zadaniaR);
            return CMax(zadaniaR);
        });
        cout << "Wynik po sortowaniu po R: " << wynikCzas2.first << " (czas: " << wynikCzas2.second << " us)" << endl;

        // CMax po sortowaniu po R i Q
        vector<Zadanie> zadaniaRQ = zadania;
        auto wynikCzas3 = zmierzCzas([&]() {
            sortRQ(zadaniaRQ);
            return CMax(zadaniaRQ);
        });
        cout << "Wynik po sortowaniu po R i Q: " << wynikCzas3.first << " (czas: " << wynikCzas3.second << " us)" << endl;

        // Algorytm Schrage
        auto wynikCzasSchrage = zmierzCzas([&]() {
            return schrage(zadania);
        });
        cout << "Wynik algorytmu Schrage: " << wynikCzasSchrage.first << " (czas: " << wynikCzasSchrage.second << " us)" << endl;

        // Algorytm Carliera
        auto wynikCzasCarlier = zmierzCzas([&]() {
            CarlierSolver solver;
            long long limitDlaInstancjiMs = (plik == "data4.txt") ? 10000 : 0;
            return solver.rozwiaz(zadania, limitDlaInstancjiMs);
        });
        cout << "Wynik algorytmu Carliera: " << wynikCzasCarlier.first.cmax
            << " (czas: " << wynikCzasCarlier.second << " us, wezly: " << wynikCzasCarlier.first.liczbaWezlow << ")";
        if (wynikCzasCarlier.first.przekroczonyLimit) {
            cout << " [przekroczono limit czasu - wynik moze nie byc optymalny]";
        }
        cout << endl;
    }
}


int main() {


    wlaczZadania();

    return 0;
}