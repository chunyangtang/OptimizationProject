# TSP Optimization

Use GASA algorithm to solve the Traveling Salesman Problem (TSP). The algorithm is implemented in C++ and the results are plotted using Python.

## Problem

The Traveling Salesman Problem (TSP) is a classic problem in combinatorial optimization. Given a list of cities and the distances between them, the task is to find the shortest possible route that visits each city exactly once and returns to the origin city.

The cities are represented by their coordinates in the 2D plane, which are stored in the files `BEN30-XY.txt`, `BEN50-XY.txt` and `BEN75-XY.txt` (first line represents the number of cities and the following lines represent the coordinates of each city).


## Algorithm

- Genetic Algorithm with Simulated Annealing (GASA)

## Usage

Use the following commands to compile and run the program and get the statistic results.

```bash
make
./main {pop} {gen} {CR} {MR} {InitT} {CoolingR} < BEN30-XY.txt # or BEN50-XY.txt or BEN75-XY.txt
```

Use the following command to compare results of different parameters.

```bash
python plot_results.py
```

Use the following command to compile a version of code that outputs detailed information about the optimization process (performance may be affected).

```bash
make collect_data
./main {pop} {gen} {CR} {MR} {InitT} {CoolingR} < BEN30-XY.txt # or BEN50-XY.txt or BEN75-XY.txt
python plot_process.py
```

Use the following command to clean the results.

```bash
make clean_data
```