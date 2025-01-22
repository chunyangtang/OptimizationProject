import glob
import os
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

sns.set_theme(style="whitegrid")

data_path = "./"
file_list = glob.glob(os.path.join(data_path, "*_results.csv"))

results = []
fitness_data = []
time_data = []
method_dict = {}

for fpath in file_list:
    fname = os.path.basename(fpath)
    method_name = fname.replace("_results.csv", "")
    df = pd.read_csv(fpath)
    
    best_fitness = df["BestFitness"].min()
    worst_fitness = df["BestFitness"].max()
    mean_fitness = df["BestFitness"].mean()
    var_fitness = df["BestFitness"].var()

    best_time = df["Time(s)"].min()
    worst_time = df["Time(s)"].max()
    mean_time = df["Time(s)"].mean()
    var_time = df["Time(s)"].var()

    results.append([method_name, mean_fitness, best_fitness, worst_fitness, var_fitness,
                    mean_time, best_time, worst_time, var_time])
    
    fitness_data.extend(df["BestFitness"].values)
    time_data.extend(df["Time(s)"].values)

res_df = pd.DataFrame(results, columns=["method", "mean_fitness", "best_fitness", "worst_fitness", "var_fitness",
                                        "mean_time", "best_time", "worst_time", "var_time"])

methods = res_df["method"].values
x = np.arange(len(methods))
width = 0.3  # 柱的宽度变窄

fig, ax1 = plt.subplots(figsize=(12, 6))
ax2 = ax1.twinx()

# 不同网格样式
ax1.grid(True, axis='y', linestyle='-', alpha=0.5, color='lightgrey')
ax1.grid(False, axis='x')

ax2.grid(True, axis='y', linestyle='--', alpha=0.5, color='lightblue')
ax2.grid(False, axis='x')

# 使用调色板
palette = sns.color_palette("pastel", 2)
fitness_color = palette[0]
time_color = palette[1]

# 计算标准差，供误差线使用
res_df["std_fitness"] = np.sqrt(res_df["var_fitness"].fillna(0))
res_df["std_time"] = np.sqrt(res_df["var_time"].fillna(0))

# 绘制Fitness柱状图（左轴）
ax1.bar(x - width/2, res_df["mean_fitness"], yerr=res_df["std_fitness"],
        width=width, color=fitness_color, edgecolor="black", alpha=0.8, 
        capsize=5, label="Mean Fitness")

# 绘制Time柱状图（右轴）
ax2.bar(x + width/2, res_df["mean_time"], yerr=res_df["std_time"],
        width=width, color=time_color, edgecolor="black", alpha=0.8, 
        capsize=5, label="Mean Time")

# 标记最佳、最差 Fitness（左轴）
for i, row in enumerate(res_df.itertuples()):
    ax1.plot(x[i] - width/2, row.best_fitness, marker="^", color="red", label="Best Fitness" if i == 0 else "")
    ax1.plot(x[i] - width/2, row.worst_fitness, marker="v", color="blue", label="Worst Fitness" if i == 0 else "")

# 标记最佳、最差 Time（右轴）
for i, row in enumerate(res_df.itertuples()):
    ax2.plot(x[i] + width/2, row.best_time, marker="^", color="darkred", label="Best Time" if i == 0 else "")
    ax2.plot(x[i] + width/2, row.worst_time, marker="v", color="darkblue", label="Worst Time" if i == 0 else "")

ax1.set_xticks(x)
ax1.set_xticklabels(methods, rotation=45, ha='right')
ax1.set_ylabel("Fitness")
ax2.set_ylabel("Time (s)")
plt.title("Comparison of Methods: Fitness (Left) and Time (Right)")

# 合并图例
handles1, labels1 = ax1.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()

handles = handles1 + handles2
labels = labels1 + labels2

# 去重图例项
unique = dict()
clean_handles = []
clean_labels = []
for h, l in zip(handles, labels):
    if l not in unique:
        unique[l] = True
        clean_handles.append(h)
        clean_labels.append(l)

ax1.legend(clean_handles, clean_labels, loc="upper left", bbox_to_anchor=(1.05, 1))

d_min_f = np.min(fitness_data)
d_max_f = np.max(fitness_data)
d_min_t = np.min(time_data)
d_max_t = np.max(time_data)

f = 0.1  # 0在轴中的比例
m = 0.1  # 在最大值上方预留0.1的空间

def compute_axis_limits(d_min, d_max, f, s):
    # 基于上方推导公式尝试给出初始y_min和y_max
    numerator = d_max * f
    denominator = f - 1 + s  # (f-1+s)
    y_min = numerator / denominator
    y_max = y_min * (1 - 1/f)

    # 检查是否覆盖d_min
    if y_min > d_min:
        # 未能覆盖数据最小值，则以d_min为下界重新计算
        y_min = d_min
        y_max = y_min*(1 - 1/f)
        # 现在0比例f仍然正确，但上方空间s可能不再完全满足
    
    return y_min, y_max

y_min_f, y_max_f = compute_axis_limits(d_min_f, d_max_f, f, m)
y_min_t, y_max_t = compute_axis_limits(d_min_t, d_max_t, f, m)

ax1.set_ylim(y_min_f, y_max_f)
ax2.set_ylim(y_min_t, y_max_t)

plt.tight_layout()
# plt.show()
plt.savefig("results_plot.png", dpi=300)