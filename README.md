# OptimizationProject

Term project for the course "Intelligent Optimization Algorithms and Its Applications" (2024 Fall)

It contains code for 2 kinds of optimization problems:
- [Function Optimization](FunctionOptimization/): Using SA, GA, PSO and DE algorithms to search for the global minimum of a given function. Rastringin function of different dimensions is used as a test example.
- [Travelling Salesman Problem](TSPOptimization/): Using GASA algorithm to solve the TSP problem of different sizes.

Both solvers are implemented in C++ and the results are plotted using Python, refer to the `readme.md` files in the respective directories for more information.

To compile and run the code, you'll need:
- C++ compiler (e.g. g++) supporting C++17 standard or later
- Python 3 with required packages listed in the [requirement](requirements.txt) file (Python 3.10 is recommended)