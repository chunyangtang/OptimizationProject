# Function Optimization

Use of optimization algorithms to find the minimum of a function. The algorithms are implemented in C++ and the results are plotted using Python.

## Function

Rastringin function or Michalewicz function in any dimension is implemented, but any other function can be used by inheriting the class `ObjectiveFunction` and implementing the method `eval`.

## Algorithms

- Simulated Annealing
- Genetic Algorithm
- Particle Swarm Optimization
- Differential Evolution

New algorithms can be implemented by inheriting the class `Optimizer`.

## Usage

Use the following commands to compile and run the program and get the statistic results.

```bash
make
./main
python plot_results.py
```

Use the following command to compile a version of code that outputs detailed information about the optimization process (performance may be affected).

```bash
make collect_data
./main
python plot_progress.py  # Animation generartion is currently commented out as it only supports 2D functions
```

Use the following command to clean the results.

```bash
make clean_data
```
