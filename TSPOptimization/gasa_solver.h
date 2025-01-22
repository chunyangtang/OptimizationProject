#ifndef GASA_SOLVER_H
#define GASA_SOLVER_H

#include <vector>
#include <random>
#include <string>
#include "tsp_instance.h"

class GASATspSolver {
public:
    // 在构造函数中接收参数
    GASATspSolver(const TSPInstance &instance,
                  int pop_size,
                  int generations,
                  double cross_rate,
                  double mutation_rate,
                  double initial_temp,
                  double cooling_rate);

    void solve();
    double getBestDistance() const;
    const std::vector<int>& getBestRoute() const;
#ifdef ENABLE_DATA_COLLECTION
    const std::vector<double>& getDistanceHistory() const;
    const std::vector<std::vector<int>>& getRouteHistory() const;
#endif

private:
    const TSPInstance &inst;
    std::mt19937 rng;

    // 参数存储为成员变量
    int popSize;
    int maxGenerations;
    double crossRate;
    double mutationRate;
    double initialTemperature;
    double coolingRate;

    std::vector<std::vector<int>> population;
    std::vector<double> fitness;

#ifdef ENABLE_DATA_COLLECTION
    std::vector<double> distance_history;
    std::vector<std::vector<int>> route_history;
#endif

    double best_distance;
    std::vector<int> best_route;

    void initializePopulation();
    void evaluateFitness();

    // 算子函数
    std::vector<int> selection();
    std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
    void mutate(std::vector<int>& individual);
    void simulatedAnnealing(std::vector<int>& route);
    // 根据算子构造下一代种群
    void nextGeneration();
};

#endif