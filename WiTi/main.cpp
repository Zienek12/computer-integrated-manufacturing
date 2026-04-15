#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <limits>

struct Job {
    int id;
    int p;
    int w;
    int d;
};

// Oblicza kar? dla zadania zakoczonego w konkretnym czasie
int calculateJobPenalty(const Job& job, int completionTime) {
    int tardiness = std::max(0, completionTime - job.d);
    return job.w * tardiness;
}

// Oblicza ca?kowit? kar? dla podanej kolejno?ci zada?.
int calculateTotalPenalty(const std::vector<Job>& jobs, const std::vector<int>& order) {
    int currentTime = 0;
    int totalPenalty = 0;

    for (int jobId : order) {
        currentTime += jobs[jobId].p;
        totalPenalty += calculateJobPenalty(jobs[jobId], currentTime);
    }

    return totalPenalty;
}

// generuje wszystkie mo?liwe permutacje
// kolejno?ci zada? i na bie??co szuka tej o najmniejszej karze.
void solveBruteForce(const std::string& name, int n, const std::vector<Job>& jobs) {
    // 1. Inicjalizacja wektora indeksów: {0, 1, 2, ..., n-1}
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);

    int minPenalty = std::numeric_limits<int>::max();
    std::vector<int> bestOrder;

    // Generowanie permutacji
    do {
        int currentPenalty = calculateTotalPenalty(jobs, order);

        if (currentPenalty < minPenalty) {
            minPenalty = currentPenalty;
            bestOrder = order;
        }
    } while (std::next_permutation(order.begin(), order.end()));

    std::cout << name << " (Przeszukiwanie Zupelne): " << minPenalty << "\nBest order: ";
    for (int idx : bestOrder) {
        std::cout << (idx + 1) << " "; 
    }
    std::cout << "\n\n";
}



void loadAndSolve(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::string line;
    while (file >> line) {
        if (line.find("data.") == 0) {
            int n;
            file >> n;
            std::vector<Job> jobs(n);
            for (int i = 0; i < n; ++i) {
                jobs[i].id = i;
                file >> jobs[i].p >> jobs[i].w >> jobs[i].d;
            }

            // algorytm Brute Force (n!) drastycznie obci??a procesor.
            if (n <= 11) {
                solveBruteForce(line, n, jobs);
            } else {
                 std::cout << line << ": pominieto ze wzgledu na dlugi czas dzialania (n > 11)\n";
            }
        }
    }
}

int main() {
    loadAndSolve("C:/Users/andos/source/repos/computer-integrated-manufacturing/WiTi/data.txt");
    return 0;
}
