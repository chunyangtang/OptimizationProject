#ifndef GA_H
#define GA_H

#include "Optimizer.h"
#include "ObjectiveFunction.h"
#include <vector>
#include <random>

class GA : public Optimizer {
public:
    GA(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double xmin, double xmax, double mutationRate, double crossoverRate);
    virtual void run() override;
    virtual const Individual& getBestIndividual() const override;

private:
    const ObjectiveFunction& objFunc;
    int DIM;
    int POP_SIZE;
    int MAX_GEN;
    double X_MIN;
    double X_MAX;
    double mutationRate;
    double crossoverRate;

    std::vector<Individual> population;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    void initializePopulation();
    void evolve();
    Individual selectParent();
    Individual crossover(const Individual& p1, const Individual& p2);
    void mutate(Individual& ind);
};

#endif // GA_H