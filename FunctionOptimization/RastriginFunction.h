#ifndef RASTRIGIN_FUNCTION_H
#define RASTRIGIN_FUNCTION_H

#include "ObjectiveFunction.h"
#include <cmath>

class RastriginFunction : public ObjectiveFunction {
public:
    RastriginFunction(int dim): dimension(dim) {}
    virtual double eval(const std::vector<double>& x) const override {
        double sum = 10 * dimension;
        for (int i = 0; i < dimension; ++i) {
            sum += x[i] * x[i] - 10 * cos(2 * M_PI * x[i]);
        }
        return sum;
    }

private:
    int dimension;
};

#endif // RASTRIGIN_FUNCTION_H