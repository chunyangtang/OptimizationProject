#ifndef TSP_INSTANCE_H
#define TSP_INSTANCE_H

#include <vector>
#include <iosfwd>

struct City {
    int x;
    int y;
};

class TSPInstance {
public:
    void loadFromStream(std::istream &in);
    double distance(int a, int b) const;
    double totalDistance(const std::vector<int>& route) const;

    const std::vector<City>& getCities() const { return cities; }

private:
    std::vector<City> cities;
};

#endif