#ifndef DE_H
#define DE_H

#include "Optimizer.h"
#include "ObjectiveFunction.h"
#include <random>

class DE : public Optimizer {
public:
    DE(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double F, double CR, double xmin, double xmax);
    virtual void run() override;
    virtual const Individual& getBestIndividual() const override;

private:
    const ObjectiveFunction& objFunc;
    int DIM;
    int POP_SIZE;
    int MAX_GEN;
    double F;
    double CR;
    double X_MIN;
    double X_MAX;

    std::vector<Individual> population;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    void initializePopulation();
    void evolve();
    Individual mutateAndCrossover(int targetIdx);
    void selectIndividual(int targetIdx, const Individual& trial);
};

#endif // DE_H