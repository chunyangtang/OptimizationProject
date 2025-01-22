#ifndef OBJECTIVE_FUNCTION_H
#define OBJECTIVE_FUNCTION_H

#include <vector>

class ObjectiveFunction {
public:
    virtual ~ObjectiveFunction() {}
    virtual double eval(const std::vector<double>& x) const = 0;
};

#endif // OBJECTIVE_FUNCTION_H