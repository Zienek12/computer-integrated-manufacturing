#include "SchedulingAlgorithms.hpp"

#include <algorithm>
#include <queue>

namespace {

struct CompareQ {
    bool operator()(const Zadanie& a, const Zadanie& b) const {
        return a.q < b.q;
    }
};

struct CompareR {
    bool operator()(const Zadanie& a, const Zadanie& b) const {
        return a.r > b.r;
    }
};

}  // namespace

int CMax(const std::vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        t = std::max(t, zadania[i].r) + zadania[i].p;
        cmax = std::max(cmax, t + zadania[i].q);
    }
    return cmax;
}

void sortR(std::vector<Zadanie>& zadania) {
    std::sort(zadania.begin(), zadania.end(), [](const Zadanie& a, const Zadanie& b) {
        return a.r < b.r;
    });
}

void sortRQ(std::vector<Zadanie>& zadania) {
    std::sort(zadania.begin(), zadania.end(), [](const Zadanie& a, const Zadanie& b) {
        if (a.r == b.r) {
            return a.q > b.q;
        }
        return a.r < b.r;
    });
}

int schrage(const std::vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareR> N;
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        N.push(zadania[i]);
    }

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareQ> G;

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
        cmax = std::max(cmax, t + j.q);
    }

    return cmax;
}

std::pair<int, std::vector<Zadanie> > schrageKolejnosc(const std::vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;
    std::vector<Zadanie> pi;

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareR> N;
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        N.push(zadania[i]);
    }

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareQ> G;

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
        cmax = std::max(cmax, t + j.q);
    }

    return std::make_pair(cmax, pi);
}

int schragePodzial(const std::vector<Zadanie>& zadania) {
    int t = 0;
    int cmax = 0;

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareR> N;
    for (std::size_t i = 0; i < zadania.size(); ++i) {
        N.push(zadania[i]);
    }

    std::priority_queue<Zadanie, std::vector<Zadanie>, CompareQ> G;
    Zadanie l = {-1, 0, 0, 0};

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
        cmax = std::max(cmax, t + j.q);
    }

    return cmax;
}
