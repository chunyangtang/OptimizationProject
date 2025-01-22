import os
import glob
import re
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

sns.set_theme(style="darkgrid")

# 获取当前目录下所有以'D'结尾的文件夹，例如2D、3D、10D等
current_dir = "."
dimension_folders = [d for d in os.listdir(current_dir) if os.path.isdir(d) and re.match(r".*D$", d)]

results = []

for folder in dimension_folders:
    # 从文件夹名称中提取维度数值
    # 假设文件夹名称为"2D"、"10D"等
    dimension_str = folder[:-1]  # 去掉最后的"D"
    try:
        dimension = int(dimension_str)
    except ValueError:
        # 如果不是纯数字+D的结构可在此处理，比如跳过
        continue

    # 找到该维度文件夹下的*_results.csv文件
    file_pattern = os.path.join(folder, "*_results.csv")
    files = glob.glob(file_pattern)

    # 对每个算法文件计算平均Fitness与Time
    for fpath in files:
        fname = os.path.basename(fpath)
        algorithm = fname.replace("_results.csv", "")
        df = pd.read_csv(fpath)

        mean_fitness = df["BestFitness"].mean()
        mean_time = df["Time(s)"].mean()

        results.append([dimension, algorithm, mean_fitness, mean_time])

# 将所有结果汇总为DataFrame
res_df = pd.DataFrame(results, columns=["dimension", "algorithm", "mean_fitness", "mean_time"])

# 根据维度排序
res_df = res_df.sort_values("dimension")

# 绘制平均Fitness随dimension变化的折线图
plt.figure(figsize=(10, 6))
sns.lineplot(data=res_df, x="dimension", y="mean_fitness", hue="algorithm", marker="o")
plt.title("Average Fitness vs Dimension")
plt.xlabel("Dimension")
plt.ylabel("Average Fitness")
plt.legend(title="Algorithm", loc="best")
plt.tight_layout()
# plt.show()
plt.savefig("average_fitness_vs_dimension.png")

# 绘制平均Time随dimension变化的折线图
plt.figure(figsize=(10, 6))
sns.lineplot(data=res_df, x="dimension", y="mean_time", hue="algorithm", marker="o")
plt.title("Average Time vs Dimension")
plt.xlabel("Dimension")
plt.ylabel("Average Time (s)")
plt.legend(title="Algorithm", loc="best")
plt.tight_layout()
# plt.show()
plt.savefig("average_time_vs_dimension.png")