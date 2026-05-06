#include <iostream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

struct Node {
    int x;
    int y;
};

class TspData {
public:
    std::vector<Node> nodes;

    void SetRand(int nodeN = 40) {
        nodes.clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, 99);
        std::uniform_int_distribution<> distY(0, 49);

        while (nodes.size() < nodeN) {
            int x = distX(gen);
            int y = distY(gen);
            bool ok = true;
            for (const auto& node : nodes) {
                int dx = x - node.x;
                int dy = y - node.y;
                if ((dx * dx + dy * dy) < 4) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                nodes.push_back({x, y});
            }
        }
    }

    int GetEdgeDist(size_t a, size_t b) const {
        int dx = nodes[a].x - nodes[b].x;
        int dy = nodes[a].y - nodes[b].y;
        return static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
    }

    int GetPathDist(const std::vector<int>& path) const {
        int length = 0;
        for (size_t i = 1; i < path.size(); ++i) {
            length += GetEdgeDist(path[i - 1], path[i]);
        }
        return length;
    }
};

class NearestNeighbor {
public:
    std::vector<int> GetPath(const TspData& tsp) {
        size_t size = tsp.nodes.size();
        std::vector<int> path;
        std::vector<bool> visited(size, false);

        int current = 0; 
        path.push_back(current);
        visited[current] = true;

        for (size_t i = 1; i < size; ++i) {
            int bestNext = -1;
            int bestDist = std::numeric_limits<int>::max();

            for (size_t j = 0; j < size; ++j) {
                if (!visited[j]) {
                    int dist = tsp.GetEdgeDist(current, j);
                    if (dist < bestDist) {
                        bestDist = dist;
                        bestNext = static_cast<int>(j);
                    }
                }
            }

            current = bestNext;
            path.push_back(current);
            visited[current] = true;
        }

        path.push_back(0);
        return path;
    }

    void RunLog(const TspData& tsp) {
        std::vector<int> path = GetPath(tsp);
        int dist = tsp.GetPathDist(path);

        std::cout << "\n--- NAJBLIZSZY SASIAD ---\n";
        std::cout << "DIST: " << dist << "\n";
        std::cout << "PATH: ";
        for (int node : path) {
            std::cout << node << " ";
        }
        std::cout << "\n\n";
    }
};

class SA {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<> distReal;

public:
    SA() : gen(std::random_device{}()), distReal(0.0, 1.0) {}

    std::vector<int> GetStartPath(size_t size) {
        std::vector<int> path(size);
        for (size_t i = 0; i < size; ++i) path[i] = i;
        path.push_back(0); 
        return path;
    }

    std::pair<int, int> GetRandAB(size_t size) {
        std::uniform_int_distribution<> d1(1, size - 1);
        int a = d1(gen);
        std::uniform_int_distribution<> d2(1, size - 2);
        int b = d2(gen);
        if (b >= a) b++;
        if (a > b) std::swap(a, b);
        return {a, b};
    }

    void SetPathMove(std::vector<int>& path, std::pair<int, int> move) {
        int a = move.first;
        int b = move.second;
        while (a < b) {
            std::swap(path[a], path[b]);
            a++;
            b--;
        }
    }

    void RunLog(const TspData& tsp) {
        double temp = 2000.0;
        std::vector<int> path = GetStartPath(tsp.nodes.size());
        int currentDist = tsp.GetPathDist(path);
        
        int bestDist = currentDist;
        std::vector<int> bestPath = path;

        std::cout << "NODES: " << tsp.nodes.size() << "\n";
        for (size_t i = 0; i < tsp.nodes.size(); ++i) {
            std::cout << "Node " << i << ": (" << tsp.nodes[i].x << ", " << tsp.nodes[i].y << ")\n";
        }
        std::cout << "--- START SA ---\n";

        for (int n = 0; n < 400; n++) {
            temp *= 0.978;
            for (int i = 0; i < 100; i++) {
                auto move = GetRandAB(tsp.nodes.size());
                SetPathMove(path, move);
                int newDist = tsp.GetPathDist(path);
                
                if (newDist < bestDist) {
                    bestDist = newDist;
                    bestPath = path;
                }

                bool accept = false;
                if (newDist > currentDist) {
                    double prob = std::exp((currentDist - newDist) / temp);
                    if (distReal(gen) < prob) {
                        accept = true;
                    }
                }

                if (newDist <= currentDist || accept) {
                    currentDist = newDist;
                } else {
                    SetPathMove(path, move); 
                }
            }

            std::cout << "STEP " << n << " DIST " << bestDist << " PATH ";
            for (int node : bestPath) std::cout << node << " ";
            std::cout << "\n";
        }
    }
};

int main() {
    TspData tspData;
    tspData.SetRand(40);

    NearestNeighbor nn;
    nn.RunLog(tspData);

    SA sa;
    sa.RunLog(tspData);

    return 0;
}