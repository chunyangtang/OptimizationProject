#include "gasa_solver.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
#include <chrono>

GASATspSolver::GASATspSolver(const TSPInstance &instance,
                             int pop_size,
                             int generations,
                             double cross_rate,
                             double mutation_rate,
                             double initial_temp,
                             double cooling_rate)
: inst(instance),
  rng((unsigned)std::chrono::system_clock::now().time_since_epoch().count()),
  popSize(pop_size),
  maxGenerations(generations),
  crossRate(cross_rate),
  mutationRate(mutation_rate),
  initialTemperature(initial_temp),
  coolingRate(cooling_rate) {
  best_distance = std::numeric_limits<double>::infinity();
}

void GASATspSolver::solve() {
    initializePopulation();
    best_distance = std::numeric_limits<double>::infinity();
#ifdef ENABLE_DATA_COLLECTION
    distance_history.clear();
    route_history.clear();
#endif

    for (int gen = 0; gen < maxGenerations; gen++) {
        evaluateFitness();
        nextGeneration();
    }
}

double GASATspSolver::getBestDistance() const {
    return best_distance;
}

const std::vector<int>& GASATspSolver::getBestRoute() const {
    return best_route;
}

#ifdef ENABLE_DATA_COLLECTION
const std::vector<double>& GASATspSolver::getDistanceHistory() const {
    return distance_history;
}

const std::vector<std::vector<int>>& GASATspSolver::getRouteHistory() const {
    return route_history;
}
#endif


void GASATspSolver::initializePopulation() {
    population.clear();
    int n = (int)inst.getCities().size();
    std::vector<int> base_route(n);
    iota(base_route.begin(), base_route.end(), 0);
    for (int i = 0; i < popSize; i++) {
        shuffle(base_route.begin(), base_route.end(), rng);
        population.push_back(base_route);
    }
}

void GASATspSolver::evaluateFitness() {
    fitness.clear();
    fitness.resize(popSize);
    for (int i = 0; i < popSize; i++) {
        double dist = inst.totalDistance(population[i]);
        fitness[i] = 1.0 / dist;
        if (dist < best_distance) {
            best_distance = dist;
            best_route = population[i];
        }
    }
#ifdef ENABLE_DATA_COLLECTION
    distance_history.push_back(best_distance);
    route_history.push_back(best_route);
#endif
}

std::vector<int> GASATspSolver::selection() {
    double fitness_sum = accumulate(fitness.begin(), fitness.end(), 0.0);
    std::uniform_real_distribution<double> dist(0.0, fitness_sum);
    double rand_point = dist(rng);
    double cumulative = 0;
    for (int i = 0; i < popSize; i++) {
        cumulative += fitness[i];
        if (cumulative >= rand_point) {
            return population[i];
        }
    }
    return population[popSize - 1];
}

std::vector<int> GASATspSolver::crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int n = (int)parent1.size();
    std::vector<int> child(n, -1);
    std::uniform_int_distribution<int> dist(0, n-1);
    int start = dist(rng);
    int end = dist(rng);
    if (start > end) std::swap(start, end);
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
    }
    for (int i = 0; i < n; i++) {
        if (find(child.begin(), child.end(), parent2[i]) == child.end()) {
            for (int j = 0; j < n; j++) {
                if (child[j] == -1) {
                    child[j] = parent2[i];
                    break;
                }
            }
        }
    }
    return child;
}

void GASATspSolver::mutate(std::vector<int>& individual) {
    int n = (int)individual.size();
    std::uniform_int_distribution<int> dist(0, n-1);
    int a = dist(rng);
    int b = dist(rng);
    std::swap(individual[a], individual[b]);
}

void GASATspSolver::simulatedAnnealing(std::vector<int>& route) {
    double temperature = initialTemperature;
    int n = (int)route.size();
    std::vector<int> current_route = route;
    double current_distance = inst.totalDistance(current_route);

    std::uniform_int_distribution<int> dist_idx(0, n-1);
    std::uniform_real_distribution<double> dist_real(0.0, 1.0);

    while (temperature > 1) {
        int a = dist_idx(rng);
        int b = dist_idx(rng);
        std::swap(current_route[a], current_route[b]);

        double new_distance = inst.totalDistance(current_route);
        double delta = new_distance - current_distance;

        if (delta < 0 || exp(-delta / temperature) > dist_real(rng)) {
            current_distance = new_distance;
        } else {
            // 撤销交换
            std::swap(current_route[a], current_route[b]);
        }

        temperature *= coolingRate;
    }

    route = current_route;
}

void GASATspSolver::nextGeneration() {
    std::vector<std::vector<int>> new_population;
    std::uniform_real_distribution<double> dist_real(0.0, 1.0);
    for (int i = 0; i < popSize; i++) {
        std::vector<int> parent1 = selection();
        std::vector<int> parent2 = selection();
        std::vector<int> child;
        if (dist_real(rng) < crossRate) {
            child = crossover(parent1, parent2);
        } else {
            child = parent1;
        }
        if (dist_real(rng) < mutationRate) {
            mutate(child);
        }
        simulatedAnnealing(child);
        new_population.push_back(child);
    }
    population = new_population;
}