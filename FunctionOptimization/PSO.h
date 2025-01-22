#ifndef PSO_H
#define PSO_H

#include "Optimizer.h"
#include "ObjectiveFunction.h"
#include <vector>
#include <random>

struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    double fitness;
    std::vector<double> bestPosition;
    double bestFitness;
};

class PSO : public Optimizer {
public:
    PSO(const ObjectiveFunction& objFunc, int dim, int popSize, int maxGen, double xmin, double xmax, double w, double c1, double c2);
    virtual void run() override;
    virtual const Individual& getBestIndividual() const override;

private:
    const ObjectiveFunction& objFunc;
    int DIM;
    int POP_SIZE;
    int MAX_GEN;
    double X_MIN;
    double X_MAX;
    double w, c1, c2;

    std::vector<Particle> swarm;
    Particle globalBest;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    void initializeSwarm();
    void updateVelocityAndPosition();
};

#endif // PSO_H