#include "SA.h"
#include <cmath>

SA::SA(const ObjectiveFunction& objFunc, int dim, int maxGen, double xmin, double xmax, double initialTemp, double coolingRate)
    : objFunc(objFunc), DIM(dim), MAX_GEN(maxGen), X_MIN(xmin), X_MAX(xmax), temp(initialTemp), coolingRate(coolingRate),
      gen(rd()), dis(0.0, 1.0)
{
    current.position.resize(DIM);
    best.position.resize(DIM);
}

void SA::initialize() {
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    for (int i = 0; i < DIM; ++i) {
        current.position[i] = dis_x(gen);
    }
    current.fitness = objFunc.eval(current.position);
    best = current;
}

Individual SA::neighbor(const Individual& ind) {
    Individual neigh = ind;
    std::uniform_real_distribution<> dis_x(X_MIN, X_MAX);
    int idx = gen() % DIM;
    neigh.position[idx] = dis_x(gen);
    neigh.fitness = objFunc.eval(neigh.position);
    return neigh;
}

double SA::acceptanceProbability(double oldFitness, double newFitness, double temperature) {
    if (newFitness < oldFitness) return 1.0;
    return std::exp((oldFitness - newFitness) / temperature);
}

void SA::run() {
    initialize();
    for (int generation = 0; generation < MAX_GEN; ++generation) {
        Individual neigh = neighbor(current);
        double ap = acceptanceProbability(current.fitness, neigh.fitness, temp);
        if (dis(gen) < ap) {
            current = neigh;
            if (current.fitness < best.fitness) {
                best = current;
            }
        }
        temp *= coolingRate;
#ifdef ENABLE_DATA_COLLECTION
        // 记录当前代数据（SA中只有一个当前解和一个最好解，这里记录当前解和最好解）
        // 我们可以约定Individual列为0表示当前解，1表示最优解
        {
            std::string line_current = std::to_string(generation) + ",0";
            for (const auto& xi : current.position) {
                line_current += "," + std::to_string(xi);
            }
            line_current += "," + std::to_string(current.fitness);
            dataBuffer.push_back(line_current);

            std::string line_best = std::to_string(generation) + ",1";
            for (const auto& xi : best.position) {
                line_best += "," + std::to_string(xi);
            }
            line_best += "," + std::to_string(best.fitness);
            dataBuffer.push_back(line_best);
        }
#endif
    }
}

const Individual& SA::getBestIndividual() const {
    return best;
}