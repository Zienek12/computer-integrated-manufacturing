#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <queue>

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
    const string path = "C:/Users/andos/source/repos/KIZ/KIZ";

    for (const auto& plik : pliki) {
        cout << "------ " << plik << " ------" << endl;
        string nazwaPliku = path + "/" + plik;
        const vector<Zadanie> zadania = wczytajZadania(nazwaPliku);

        // CMax bez sortowania
        auto [wynik1, czas1] = zmierzCzas([&]() { return CMax(zadania); });
        cout << "Wynik dla sortowania 123..: " << wynik1 << " (czas: " << czas1 << " us)" << endl;

        // CMax po sortowaniu po R
        vector<Zadanie> zadaniaR = zadania;
        auto [wynik2, czas2] = zmierzCzas([&]() {
            sortR(zadaniaR);
            return CMax(zadaniaR);
        });
        cout << "Wynik po sortowaniu po R: " << wynik2 << " (czas: " << czas2 << " us)" << endl;

        // CMax po sortowaniu po R i Q
        vector<Zadanie> zadaniaRQ = zadania;
        auto [wynik3, czas3] = zmierzCzas([&]() {
            sortRQ(zadaniaRQ);
            return CMax(zadaniaRQ);
        });
        cout << "Wynik po sortowaniu po R i Q: " << wynik3 << " (czas: " << czas3 << " us)" << endl;

        // Algorytm Schrage
        auto [wynikSchrage, czasSchrage] = zmierzCzas([&]() {
            return schrage(zadania);
        });
        cout << "Wynik algorytmu Schrage: " << wynikSchrage << " (czas: " << czasSchrage << " us)" << endl;
    }
}


int main() {


    wlaczZadania();

    return 0;
}