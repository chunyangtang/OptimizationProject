#ifndef MICHALIEWICZ_FUNCTION_H
#define MICHALIEWICZ_FUNCTION_H

#include "ObjectiveFunction.h"
#include <cmath>

class MichalewiczFunction : public ObjectiveFunction {
public:
    MichalewiczFunction(int dim, int m = 10): dimension(dim), M(m) {}
    virtual double eval(const std::vector<double>& x) const override {
        double sum = 0.0;
        for (int i = 0; i < dimension; ++i) {
            sum += sin(x[i]) * std::pow(sin(((i + 1) * x[i] * x[i]) / M_PI), 2 * M);
        }
        return -sum;
    }

private:
    int dimension;
    int M;
};

#endif // MICHALIEWICZ_FUNCTION_H