#include "tsp_instance.h"
#include <cmath>
#include <istream>

void TSPInstance::loadFromStream(std::istream &in) {
    int n;
    in >> n;
    cities.resize(n);
    for (int i = 0; i < n; i++) {
        in >> cities[i].x >> cities[i].y;
    }
}

double TSPInstance::distance(int a, int b) const {
    int dx = cities[a].x - cities[b].x;
    int dy = cities[a].y - cities[b].y;
    return std::sqrt(dx * dx + dy * dy);
}

double TSPInstance::totalDistance(const std::vector<int>& route) const {
    double sum = 0;
    for (int i = 0; i < (int)route.size() - 1; i++) {
        sum += distance(route[i], route[i + 1]);
    }
    sum += distance(route.back(), route[0]); // 回到起点
    return sum;
}