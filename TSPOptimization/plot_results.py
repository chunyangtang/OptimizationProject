import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import os
import re

# Set Seaborn style
sns.set_theme(style="darkgrid")

# Get all directories with the prefix 'results_'
result_dirs = [d for d in os.listdir('.') if os.path.isdir(d) and d.startswith('results_')]

def natural_sort_key(s):
    """
    自然排序的关键生成函数：将字符串中的数字与非数字部分分开处理
    """
    return [int(text) if text.isdigit() else text.lower() for text in re.split(r'(\d+)', s)]

result_dirs.sort(key=natural_sort_key)

all_results = []

for folder_name in result_dirs:
    # Load the data
    best_routes_path = os.path.join(folder_name, "best_routes.csv")
    results_path = os.path.join(folder_name, "results.csv")

    if not os.path.exists(best_routes_path) or not os.path.exists(results_path):
        continue

    best_routes_df = pd.read_csv(best_routes_path)
    results_df = pd.read_csv(results_path)

    # Extract parameters from folder name
    params = folder_name.replace("results_", "").split("_")
    pop_size, generations, cross_rate, mutation_rate, initial_temp, cooling_rate, cities = params
    pop_size = int(pop_size)
    generations = int(generations)
    cross_rate = float(cross_rate)
    mutation_rate = float(mutation_rate)
    initial_temp = float(initial_temp)
    cooling_rate = float(cooling_rate)
    cities = int(cities)

    # Calculate statistics for BestDistance and Time(s)
    best_distance_stats = {
        "mean": results_df["BestDistance"].mean(),
        "min": results_df["BestDistance"].min(),
        "max": results_df["BestDistance"].max(),
        "variance": results_df["BestDistance"].var(),
    }
    time_stats = {
        "mean": results_df["Time(s)"].mean(),
        "min": results_df["Time(s)"].min(),
        "max": results_df["Time(s)"].max(),
        "variance": results_df["Time(s)"].var(),
    }

    all_results.append({
        "folder": folder_name,
        "pop_size": pop_size,
        "generations": generations,
        "cross_rate": cross_rate,
        "mutation_rate": mutation_rate,
        "initial_temp": initial_temp,
        "cooling_rate": cooling_rate,
        "cities": cities,
        "mean_distance": best_distance_stats["mean"],
        "min_distance": best_distance_stats["min"],
        "max_distance": best_distance_stats["max"],
        "variance_distance": best_distance_stats["variance"],
        "mean_time": time_stats["mean"],
        "min_time": time_stats["min"],
        "max_time": time_stats["max"],
        "variance_time": time_stats["variance"]
    })

    # Plot the distribution of BestDistance and Time(s)
    plt.figure(figsize=(12, 6))
    plt.scatter(results_df["Time(s)"], results_df["BestDistance"], alpha=0.7, color=sns.color_palette("tab10")[0], label="Data points")

    # Add horizontal and vertical lines for mean values
    plt.axhline(best_distance_stats["mean"], color=sns.color_palette("tab10")[1], linestyle="--", label=f"Mean Distance: {best_distance_stats['mean']:.2f}")
    plt.axvline(time_stats["mean"], color=sns.color_palette("tab10")[2], linestyle="--", label=f"Mean Time: {time_stats['mean']:.2f}s")

    # Add text outside the plot area for statistics
    plt.text(1.02, 0.5, 
             f"Distance\nMin: {best_distance_stats['min']:.2f}\nMax: {best_distance_stats['max']:.2f}\nVar: {best_distance_stats['variance']:.2f}\n\n"
             f"Time\nMin: {time_stats['min']:.2f}s\nMax: {time_stats['max']:.2f}s\nVar: {time_stats['variance']:.2f}s²", 
             transform=plt.gca().transAxes, fontsize=12, verticalalignment='center', bbox=dict(facecolor='white', alpha=0.8))

    # Add labels, title, and legend
    plt.title("Distribution of BestDistance and Time(s)")
    plt.xlabel("Time (s)")
    plt.ylabel("Best Distance")
    plt.legend()
    plt.grid()
    plt.tight_layout()
    plt.savefig(os.path.join(folder_name, "distribution_plot.png"))
    # plt.show()
    plt.close()

    # Extract the optimal route based on the minimum distance
    optimal_run = results_df.loc[results_df["BestDistance"].idxmin()]
    optimal_route_data = best_routes_df.loc[best_routes_df["Run"] == optimal_run["Run"], "Route"].values[0]

    # Parse the coordinates from the Route column
    coordinates = list(map(float, optimal_route_data.split()))
    coordinates = np.array(coordinates).reshape(-1, 2)

    # Plot the optimal TSP route
    plt.figure(figsize=(8, 8))

    for i in range(len(coordinates)):
        start = coordinates[i]
        end = coordinates[(i + 1) % len(coordinates)]
        plt.plot([start[0], end[0]], [start[1], end[1]], color="lightblue", zorder=1)  # Add lines between nodes

    plt.scatter(coordinates[:, 0], coordinates[:, 1], color="tab:blue", label="Nodes", zorder=2)  # Add nodes

    # for i, (x, y) in enumerate(coordinates):
    #     plt.text(x, y, f"{i}", fontsize=8, color="red")  # Add node indices in red

    # Add title and labels
    plt.title(f"Optimal TSP Route (Run {optimal_run['Run']}, Distance: {optimal_run['BestDistance']:.2f})")
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.legend()
    plt.grid()
    plt.tight_layout()
    plt.savefig(os.path.join(folder_name, "optimal_tsp_route.png"))
    # plt.show()
    plt.close()

# # Convert all results to DataFrame
# all_results_df = pd.DataFrame(all_results)

# # Plot the comparison of mean distance and mean time
# # methods = all_results_df["folder"].values
# # Extract method names based on the unique parameter settings (only the different ones)
# method_names_dict = {
#     "pop_size": "Pop",
#     "generations": "Gens",
#     "cross_rate": "CR",
#     "mutation_rate": "MR",
#     "initial_temp": "InitT",
#     "cooling_rate": "CoolingR"
# }

# methods = ["" for _ in range(len(all_results_df))]

# for param in method_names_dict:
#     # check if all items are the same
#     if all_results_df[param].nunique() == 1:
#         continue
#     else:
#         for i, value in enumerate(all_results_df[param]):
#             methods[i] += f"{method_names_dict[param]}:{value} "


# x = np.arange(len(methods))
# width = 0.3  # Width of the bars

# fig, ax1 = plt.subplots(figsize=(12, 6))
# ax2 = ax1.twinx()

# # Different grid styles
# ax1.grid(True, axis='y', linestyle='-', alpha=0.5, color='lightgrey')
# ax1.grid(False, axis='x')

# ax2.grid(True, axis='y', linestyle='--', alpha=0.5, color='lightblue')
# ax2.grid(False, axis='x')

# # Use color palette
# palette = sns.color_palette("pastel", 2)
# distance_color = palette[0]
# time_color = palette[1]

# # Calculate standard deviation for error bars
# all_results_df["std_distance"] = np.sqrt(all_results_df["variance_distance"].fillna(0))
# all_results_df["std_time"] = np.sqrt(all_results_df["variance_time"].fillna(0))

# # Plot mean distance bars (left axis)
# ax1.bar(x - width/2, all_results_df["mean_distance"], yerr=all_results_df["std_distance"],
#         width=width, color=distance_color, edgecolor="black", alpha=0.8, 
#         capsize=5, label="Mean Distance")

# # Plot mean time bars (right axis)
# ax2.bar(x + width/2, all_results_df["mean_time"], yerr=all_results_df["std_time"],
#         width=width, color=time_color, edgecolor="black", alpha=0.8, 
#         capsize=5, label="Mean Time")

# # Mark best and worst distances (left axis)
# for i, row in enumerate(all_results_df.itertuples()):
#     ax1.plot(x[i] - width/2, row.min_distance, marker="^", color="red", label="Best Distance" if i == 0 else "")
#     ax1.plot(x[i] - width/2, row.max_distance, marker="v", color="blue", label="Worst Distance" if i == 0 else "")

# # Mark best and worst times (right axis)
# for i, row in enumerate(all_results_df.itertuples()):
#     ax2.plot(x[i] + width/2, row.min_time, marker="^", color="darkred", label="Best Time" if i == 0 else "")
#     ax2.plot(x[i] + width/2, row.max_time, marker="v", color="darkblue", label="Worst Time" if i == 0 else "")

# # Add labels and title
# ax1.set_xticks(x)
# ax1.set_xticklabels(methods, rotation=45, ha='right')

# ax1.set_ylabel("Distance")
# ax2.set_ylabel("Time (s)")
# plt.title("Comparison of Different Parameter Settings: Distance (Left) and Time (Right)")

# # Combine legends
# handles1, labels1 = ax1.get_legend_handles_labels()
# handles2, labels2 = ax2.get_legend_handles_labels()

# handles = handles1 + handles2
# labels = labels1 + labels2

# # Remove duplicate legend entries
# unique = dict()
# clean_handles = []
# clean_labels = []
# for h, l in zip(handles, labels):
#     if l not in unique:
#         unique[l] = True
#         clean_handles.append(h)
#         clean_labels.append(l)

# ax1.legend(clean_handles, clean_labels, loc="upper left", bbox_to_anchor=(1.05, 1))

# plt.tight_layout()
# plt.savefig("comparison_plot.png", dpi=300)
# plt.show()