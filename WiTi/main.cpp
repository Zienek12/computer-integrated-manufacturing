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

int calculateJobPenalty(const Job& job, int completionTime) {
    int tardiness = std::max(0, completionTime - job.d);
    return job.w * tardiness;
}

int calculateTotalPenalty(const std::vector<Job>& jobs, const std::vector<int>& order) {
    int currentTime = 0;
    int totalPenalty = 0;

    for (int jobId : order) {
        currentTime += jobs[jobId].p;
        totalPenalty += calculateJobPenalty(jobs[jobId], currentTime);
    }

    return totalPenalty;
}

void solveBruteForce(const std::string& name, int n, const std::vector<Job>& jobs) {
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);

    int minPenalty = std::numeric_limits<int>::max();
    std::vector<int> bestOrder;

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

std::vector<int> calculateCompletionTimes(int n, int num_masks, const std::vector<Job>& jobs) {
    std::vector<int> c(num_masks, 0);
    for (int mask = 1; mask < num_masks; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                c[mask] += jobs[i].p;
            }
        }
    }
    return c;
}

void computeDPCore(int n, int num_masks, const std::vector<Job>& jobs, const std::vector<int>& c, 
                   std::vector<int>& dp, std::vector<int>& parent, std::vector<int>& lastJob) {
    for (int mask = 1; mask < num_masks; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) { 
                int prev_mask = mask ^ (1 << i); 
                int penalty = calculateJobPenalty(jobs[i], c[mask]);
                int cost = dp[prev_mask] + penalty;

                if (cost < dp[mask]) {
                    dp[mask] = cost;
                    parent[mask] = prev_mask; 
                    lastJob[mask] = i;        
                }
            }
        }
    }
}

std::vector<int> reconstructBestOrder(int num_masks, const std::vector<int>& parent, const std::vector<int>& lastJob) {
    std::vector<int> bestOrder;
    int curr_mask = num_masks - 1; 
    while (curr_mask > 0) {
        bestOrder.push_back(lastJob[curr_mask]);
        curr_mask = parent[curr_mask];
    }
    std::reverse(bestOrder.begin(), bestOrder.end()); 
    return bestOrder;
}

void solveDP(const std::string& name, int n, const std::vector<Job>& jobs) {
    int num_masks = 1 << n; 
    std::vector<int> dp(num_masks, 1e9);
    dp[0] = 0;

    std::vector<int> parent(num_masks, -1);
    std::vector<int> lastJob(num_masks, -1);

    std::vector<int> c = calculateCompletionTimes(n, num_masks, jobs);

    computeDPCore(n, num_masks, jobs, c, dp, parent, lastJob);

    std::vector<int> bestOrder = reconstructBestOrder(num_masks, parent, lastJob);

    std::cout << name << " (Dynamic programming): " << dp[num_masks - 1] << "\nBest order: ";
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

            solveDP(line, n, jobs);

        }
    }
}

int main() {
    loadAndSolve("../../../../WiTi/data.txt");
    return 0;
}
