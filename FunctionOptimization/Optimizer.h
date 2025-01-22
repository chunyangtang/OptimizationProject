#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#ifdef ENABLE_DATA_COLLECTION
#include <string>
#endif

struct Individual {
    std::vector<double> position;
    double fitness;
};

class Optimizer {
public:
    virtual ~Optimizer() {}
    virtual void run() = 0;
    virtual const Individual& getBestIndividual() const = 0;
#ifdef ENABLE_DATA_COLLECTION
    virtual const std::vector<std::string> getDataBuffer() const { 
        std::string header = "Generation,Individual";
        for (size_t d = 1; d <= getBestIndividual().position.size(); ++d) {
            header += ",x" + std::to_string(d);
        }
        header += ",Fitness";
        std::vector<std::string> modifiedDataBuffer({ header });
        modifiedDataBuffer.insert(modifiedDataBuffer.end(), dataBuffer.begin(), dataBuffer.end());
        return modifiedDataBuffer;
    }
    virtual void clearDataBuffer() { dataBuffer.clear(); }
#endif
protected:
#ifdef ENABLE_DATA_COLLECTION
    std::vector<std::string> dataBuffer;
#endif
};

#endif // OPTIMIZER_H