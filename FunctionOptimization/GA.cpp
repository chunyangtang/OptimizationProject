#include "GA.h"
#include <algorithm>
#include <cmath>

GA::GA(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double xmin, double xmax, double mutationRate, double crossoverRate)
    : objFunc(objFunc), DIM(dim), POP_SIZE(popSize), MAX_GEN(maxGen), X_MIN(xmin), X_MAX(xmax),
      mutationRate(mutationRate), crossoverRate(crossoverRate), gen(rd()), dis(0.0, 1.0)
{
    population.resize(POP_SIZE);
}

void GA::initializePopulation() {
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    for (auto& ind : population) {
        ind.position.resize(DIM);
        for (auto& xi : ind.position) {
            xi = dis_x(gen);
        }
        ind.fitness = objFunc.eval(ind.position);
    }
}

Individual GA::selectParent() {
    // 简单锦标赛选择
    int a = gen() % POP_SIZE;
    int b = gen() % POP_SIZE;
    return (population[a].fitness < population[b].fitness) ? population[a] : population[b];
}

Individual GA::crossover(const Individual& p1, const Individual& p2) {
    Individual child;
    child.position.resize(DIM);
    int cp = gen() % DIM;
    for (int i = 0; i < DIM; ++i) {
        if (i < cp) child.position[i] = p1.position[i];
        else child.position[i] = p2.position[i];
    }
    return child;
}

void GA::mutate(Individual& ind) {
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    for (int i = 0; i < DIM; ++i) {
        if (dis(gen) < mutationRate) {
            ind.position[i] = dis_x(gen);
        }
    }
}

void GA::evolve() {
    std::vector<Individual> newPop;
    newPop.reserve(POP_SIZE);
    for (int i = 0; i < POP_SIZE; ++i) {
        Individual p1 = selectParent();
        Individual p2 = selectParent();
        Individual child = p1;
        if (dis(gen) < crossoverRate) {
            child = crossover(p1, p2);
        }
        mutate(child);
        child.fitness = objFunc.eval(child.position);
        newPop.push_back(child);
    }
    population = newPop;
}

void GA::run() {
    initializePopulation();
    for (int generation = 0; generation < MAX_GEN; ++generation) {
        evolve();
#ifdef ENABLE_DATA_COLLECTION
        for (int i = 0; i < POP_SIZE; ++i) {
            std::string line = std::to_string(generation) + "," + std::to_string(i);
            for (const auto& xi : population[i].position) {
                line += "," + std::to_string(xi);
            }
            line += "," + std::to_string(population[i].fitness);
            dataBuffer.push_back(line);
        }
#endif
    }
}

const Individual& GA::getBestIndividual() const {
    return *std::min_element(population.begin(), population.end(),
                             [](const Individual& a, const Individual& b) {
                                 return a.fitness < b.fitness;
                             });
}