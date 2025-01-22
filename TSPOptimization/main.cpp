#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <filesystem>
#include <fstream>
#include "tsp_instance.h"
#include "gasa_solver.h"

using namespace std;
namespace fs = filesystem;

const int runs = 20;             // 运行次数

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " <pop_size> <generations> <cross_rate> <mutation_rate> <initial_temp> <cooling_rate>\n";
        return 1;
    }

    int pop_size = stoi(argv[1]);
    int generations = stoi(argv[2]);
    double cross_rate = stod(argv[3]);
    double mutation_rate = stod(argv[4]);
    double initial_temp = stod(argv[5]);
    double cooling_rate = stod(argv[6]);

    string folder_name = "results_" + to_string(pop_size) + "_" + to_string(generations) + "_" + to_string(cross_rate) + "_" + to_string(mutation_rate) + "_" + to_string(initial_temp) + "_" + to_string(cooling_rate);
    fs::create_directory(folder_name);

    TSPInstance instance;
    instance.loadFromStream(cin);

    vector<double> results;
    results.reserve(runs);

    vector<vector<int>> routes;
    routes.reserve(runs);

    vector<double> times;
    times.reserve(runs);

#ifdef ENABLE_DATA_COLLECTION
    // 创建文件夹存储每次运行的种群数据
    fs::create_directory(folder_name + "/data");
#endif

    for (int i = 0; i < runs; i++) {
        auto start = chrono::high_resolution_clock::now();

        // 指定参数进行初始化
        GASATspSolver solver(instance, pop_size, generations,
                             cross_rate, mutation_rate,
                             initial_temp, cooling_rate);
        solver.solve();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;
        double elapsed_time = diff.count();
        times.push_back(elapsed_time);

        double best_dist = solver.getBestDistance();
        results.push_back(best_dist);
        vector<int> best_route = solver.getBestRoute();
        routes.push_back(best_route);
#ifdef ENABLE_DATA_COLLECTION
        // 输出路径与距离历史
        const vector<double>& distance_history = solver.getDistanceHistory();
        const vector<vector<int>>& route_history = solver.getRouteHistory();
        ofstream dist_file(folder_name + "/data/distance_history_run_" + to_string(i) + ".csv");
        for (size_t j = 0; j < distance_history.size(); j++) {
            dist_file << j << "," << distance_history[j] << "\n";
        }
        dist_file.close();
        ofstream route_file(folder_name + "/data/route_history_run_" + to_string(i) + ".csv");
        for (size_t j = 0; j < route_history.size(); j++) {
            for (size_t k = 0; k < route_history[j].size(); k++) {
                int city = route_history[j][k];
                route_file << instance.getCities()[city].x << " " << instance.getCities()[city].y << ",";
            }
            route_file << "\n";
        }
        route_file.close();
#endif
    }

    // 统计结果
    double sum = accumulate(results.begin(), results.end(), 0.0);
    double avg = sum / runs;
    double best_perf = *min_element(results.begin(), results.end());
    double worst_perf = *max_element(results.begin(), results.end());

    double var_sum = 0.0;
    for (auto val : results) {
        var_sum += (val - avg) * (val - avg);
    }
    double variance = var_sum / runs;

    double time_sum = accumulate(times.begin(), times.end(), 0.0);
    double avg_time = time_sum / runs;
    double best_time = *min_element(times.begin(), times.end());
    double worst_time = *max_element(times.begin(), times.end());

    double time_var_sum = 0.0;
    for (auto t : times) {
        time_var_sum += (t - avg_time) * (t - avg_time);
    }
    double time_variance = time_var_sum / runs;

    ofstream result_file(folder_name + "/results.csv");
    result_file << "Run,BestDistance,Time(s)\n";
    for (int i = 0; i < runs; i++) {
        result_file << i << "," << results[i] << "," << times[i] << "\n";
    }
    result_file.close();

    ofstream route_file(folder_name + "/best_routes.csv");
    route_file << "Run,Route\n";
    for (int i = 0; i < runs; i++) {
        route_file << i << ",";
        for (size_t j = 0; j < routes[i].size(); j++) {
            route_file << instance.getCities()[routes[i][j]].x << " " << instance.getCities()[routes[i][j]].y << " ";
        }
        route_file << "\n";
    }
    route_file.close();

    ofstream stats_file(folder_name + "/statistics.txt");
    stats_file << "Number of Runs: " << runs << "\n";
    stats_file << "Best Distance: " << best_perf << "\n";
    stats_file << "Worst Distance: " << worst_perf << "\n";
    stats_file << "Average Distance: " << avg << "\n";
    stats_file << "Distance Variance: " << variance << "\n";
    stats_file << "Best Time: " << best_time << "\n";
    stats_file << "Worst Time: " << worst_time << "\n";
    stats_file << "Average Time: " << avg_time << "\n";
    stats_file << "Time Variance: " << time_variance << "\n";
    stats_file.close();

    return 0;
}