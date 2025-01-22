#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <filesystem>

#include "ObjectiveFunction.h"
#include "Optimizer.h"
#include "RastriginFunction.h"
// #include "MichalewiczFunction.h"
#include "DE.h"
#include "GA.h"
#include "SA.h"
#include "PSO.h"

namespace fs = std::filesystem;

// 参数
const int DIM = 10;           // 问题的维度
const int POP_SIZE = 50;     // 种群规模
const int MAX_GEN = 500;     // 最大迭代次数
const double X_MIN = -5.12;  // 变量最小值
const double X_MAX = 5.12;   // 变量最大值

const int NUM_RUNS = 20;     // 运行次数

int main() {

    RastriginFunction objFunc(DIM);

    // 模拟退火参数：目标函数、维度、最大迭代次数、变量最小值、变量最大值、初始温度、冷却速率
    SA sa(objFunc, DIM, MAX_GEN, X_MIN, X_MAX, 100.0, 0.99);

    // 遗传算法参数：目标函数、维度、种群规模、最大迭代次数、变异概率、交叉概率
    GA ga(objFunc, DIM, POP_SIZE, MAX_GEN, X_MIN, X_MAX, 0.1, 0.8);

    // 粒子群参数：目标函数、维度、种群规模、最大迭代次数、变量最小值、变量最大值、惯性权重、个体学习因子、社会学习因子
    PSO pso(objFunc, DIM, POP_SIZE, MAX_GEN, X_MIN, X_MAX, 0.5, 1.5, 1.5);

    // 差分进化参数：目标函数、维度、种群规模、最大迭代次数、差分变异因子、交叉概率、变量最小值、变量最大值
    DE de(objFunc, DIM, POP_SIZE, MAX_GEN, 0.5, 0.9, X_MIN, X_MAX);

    // 比较多种优化算法
    std::vector<Optimizer*> optimizers = { &sa, &ga, &pso, &de };
    std::vector<std::string> names = { "SA", "GA", "PSO", "DE" };

    for (size_t i = 0; i < optimizers.size(); ++i) {
        // 打开文件记录总体结果
        std::ofstream result_file(names[i] + "_results.csv");
        result_file << "Run,BestFitness,Time(s)\n";
        // 存储每次运行的最佳适应度值和耗时
        std::vector<double> best_fitness_all_runs;
        std::vector<double> time_all_runs;
#ifdef ENABLE_DATA_COLLECTION
        // 创建文件夹存储每次运行的种群数据
        fs::create_directory(names[i] + "_data");
#endif
        for (int run = 1; run <= NUM_RUNS; ++run) {
            // 记录开始时间
            auto start_time = std::chrono::high_resolution_clock::now();
            // 运行优化算法
            optimizers[i]->run();
            // 记录结束时间
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
            // 计算本次运行的最佳适应度值
            double best_fitness = optimizers[i]->getBestIndividual().fitness;
            // 记录本次运行的结果
            result_file << run << "," << best_fitness << "," << elapsed_seconds << "\n";
            best_fitness_all_runs.push_back(best_fitness);
            time_all_runs.push_back(elapsed_seconds);
            
#ifdef ENABLE_DATA_COLLECTION
            // 优化结束后，将数据写入文件，这部分时间不计入优化时间
            std::ofstream pop_file(names[i] + "_data/population_run_" + std::to_string(run) + ".csv");
            for (const auto& line : optimizers[i]->getDataBuffer()) {
                pop_file << line << "\n";
            }
            pop_file.close();
            optimizers[i]->clearDataBuffer();
#endif
        }
        result_file.close();

        // 计算统计指标
        double sum_fitness = std::accumulate(best_fitness_all_runs.begin(), best_fitness_all_runs.end(), 0.0);
        double avg_fitness = sum_fitness / NUM_RUNS;
        double best_overall = *std::min_element(best_fitness_all_runs.begin(), best_fitness_all_runs.end());
        double worst_overall = *std::max_element(best_fitness_all_runs.begin(), best_fitness_all_runs.end());

        // 计算方差
        double sq_sum = std::inner_product(best_fitness_all_runs.begin(), best_fitness_all_runs.end(), best_fitness_all_runs.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / NUM_RUNS - avg_fitness * avg_fitness);

        // 输出统计结果
        std::ofstream stats_file(names[i] + "_statistics.txt");
        stats_file << "Differential Evolution Optimization Statistics\n";
        stats_file << "Number of Runs: " << NUM_RUNS << "\n";
        stats_file << "Best Fitness: " << best_overall << "\n";
        stats_file << "Worst Fitness: " << worst_overall << "\n";
        stats_file << "Average Fitness: " << avg_fitness << "\n";
        stats_file << "Standard Deviation: " << stdev << "\n";

        // 同样计算时间的平均值和方差
        double sum_time = std::accumulate(time_all_runs.begin(), time_all_runs.end(), 0.0);
        double avg_time = sum_time / NUM_RUNS;
        double sq_sum_time = std::inner_product(time_all_runs.begin(), time_all_runs.end(), time_all_runs.begin(), 0.0);
        double stdev_time = std::sqrt(sq_sum_time / NUM_RUNS - avg_time * avg_time);

        stats_file << "Average Time(s): " << avg_time << "\n";
        stats_file << "Time Standard Deviation(s): " << stdev_time << "\n";

        stats_file.close();


    }

    return 0;
}