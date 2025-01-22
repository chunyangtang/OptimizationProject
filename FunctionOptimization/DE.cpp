#include "DE.h"
#include <algorithm>
#include <cmath>

DE::DE(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double F, double CR, double xmin, double xmax)
    : objFunc(objFunc), DIM(dim), POP_SIZE(popSize), MAX_GEN(maxGen), F(F), CR(CR), X_MIN(xmin), X_MAX(xmax),
      gen(rd()), dis(0.0, 1.0)
{
    population.resize(POP_SIZE);
}

void DE::initializePopulation() {
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    for (auto& ind : population) {
        ind.position.resize(DIM);
        for (auto& xi : ind.position) {
            xi = dis_x(gen);
        }
        ind.fitness = objFunc.eval(ind.position);
    }
}

void DE::evolve() {
    for (int i = 0; i < POP_SIZE; ++i) {
        Individual trial = mutateAndCrossover(i);
        selectIndividual(i, trial);
    }
}

Individual DE::mutateAndCrossover(int targetIdx) {
    int a, b, c;
    do { a = gen() % POP_SIZE; } while (a == targetIdx);
    do { b = gen() % POP_SIZE; } while (b == targetIdx || b == a);
    do { c = gen() % POP_SIZE; } while (c == targetIdx || c == a || c == b);

    Individual trial;
    trial.position.resize(DIM);
    int rand_idx = gen() % DIM;
    for (int j = 0; j < DIM; ++j) {
        if (dis(gen) < CR || j == rand_idx) {
            trial.position[j] = population[a].position[j] + F * (population[b].position[j] - population[c].position[j]);
            if (trial.position[j] < X_MIN) trial.position[j] = X_MIN;
            if (trial.position[j] > X_MAX) trial.position[j] = X_MAX;
        } else {
            trial.position[j] = population[targetIdx].position[j];
        }
    }
    trial.fitness = objFunc.eval(trial.position);
    return trial;
}

void DE::selectIndividual(int targetIdx, const Individual& trial) {
    if (trial.fitness < population[targetIdx].fitness) {
        population[targetIdx] = trial;
    }
}

void DE::run() {
    initializePopulation();
    for (int generation = 0; generation < MAX_GEN; ++generation) {
        evolve();
#ifdef ENABLE_DATA_COLLECTION
        // 记录当前代的种群数据
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

const Individual& DE::getBestIndividual() const {
    return *std::min_element(population.begin(), population.end(),
                             [](const Individual& a, const Individual& b) {
                                 return a.fitness < b.fitness;
                             });
}