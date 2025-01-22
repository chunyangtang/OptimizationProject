#ifndef SA_H
#define SA_H

#include "Optimizer.h"
#include "ObjectiveFunction.h"
#include <vector>
#include <random>

class SA : public Optimizer {
public:
    SA(const ObjectiveFunction& objFunc, int dim, int maxGen, double xmin, double xmax, double initialTemp, double coolingRate);
    virtual void run() override;
    virtual const Individual& getBestIndividual() const override;

private:
    const ObjectiveFunction& objFunc;
    int DIM;
    int MAX_GEN;
    double X_MIN;
    double X_MAX;
    double temp;
    double coolingRate;

    Individual current;
    Individual best;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    void initialize();
    Individual neighbor(const Individual& ind);
    double acceptanceProbability(double oldFitness, double newFitness, double temperature);
};

#endif // SA_H