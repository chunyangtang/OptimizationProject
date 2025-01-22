#include "PSO.h"
#include <algorithm>
#include <cmath>

PSO::PSO(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double xmin, double xmax, double w, double c1, double c2)
    : objFunc(objFunc), DIM(dim), POP_SIZE(popSize), MAX_GEN(maxGen), X_MIN(xmin), X_MAX(xmax), w(w), c1(c1), c2(c2),
      gen(rd()), dis(0.0, 1.0)
{
    swarm.resize(POP_SIZE);
    globalBest.bestFitness = std::numeric_limits<double>::infinity();
#ifdef ENABLE_DATA_COLLECTION
    // 写入表头
    std::string header = "Generation,Individual";
    for (int d = 1; d <= DIM; ++d) {
        header += ",x" + std::to_string(d);
    }
    header += ",Fitness";
    dataBuffer.push_back(header);
#endif
}

void PSO::initializeSwarm() {
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    for (auto& p : swarm) {
        p.position.resize(DIM);
        p.velocity.resize(DIM, 0.0);
        for (int i = 0; i < DIM; ++i) {
            p.position[i] = dis_x(gen);
        }
        p.fitness = objFunc.eval(p.position);
        p.bestPosition = p.position;
        p.bestFitness = p.fitness;

        if (p.fitness < globalBest.bestFitness) {
            globalBest = p;
        }
    }
}

void PSO::updateVelocityAndPosition() {
    for (auto& p : swarm) {
        for (int i = 0; i < DIM; ++i) {
            double r1 = dis(gen);
            double r2 = dis(gen);
            p.velocity[i] = w * p.velocity[i] + c1 * r1 * (p.bestPosition[i] - p.position[i]) + c2 * r2 * (globalBest.position[i] - p.position[i]);
            p.position[i] += p.velocity[i];
            if (p.position[i] < X_MIN) p.position[i] = X_MIN;
            if (p.position[i] > X_MAX) p.position[i] = X_MAX;
        }
        p.fitness = objFunc.eval(p.position);
        if (p.fitness < p.bestFitness) {
            p.bestFitness = p.fitness;
            p.bestPosition = p.position;
        }
        if (p.fitness < globalBest.bestFitness) {
            globalBest = p;
        }
    }
}

void PSO::run() {
    initializeSwarm();
    for (int generation = 0; generation < MAX_GEN; ++generation) {
        updateVelocityAndPosition();
#ifdef ENABLE_DATA_COLLECTION
        for (int i = 0; i < POP_SIZE; ++i) {
            std::string line = std::to_string(generation) + "," + std::to_string(i);
            for (const auto& xi : swarm[i].position) {
                line += "," + std::to_string(xi);
            }
            line += "," + std::to_string(swarm[i].fitness);
            dataBuffer.push_back(line);
        }
#endif
    }
}

const Individual& PSO::getBestIndividual() const {
    static Individual ind; 
    ind.position = globalBest.position;
    ind.fitness = globalBest.bestFitness;
    return ind;
}