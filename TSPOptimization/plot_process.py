import os
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Step 1: 获取文件夹路径并过滤有效的结果
def get_valid_folders(base_path="."):
    valid_folders = []
    for folder in os.listdir(base_path):
        if folder.startswith("results_"):
            data_path = os.path.join(base_path, folder, "data")
            if os.path.isdir(data_path):
                valid_folders.append(data_path)
    return valid_folders

# Step 2: 读取距离历史数据并找到收敛距离最优的一次
def get_best_run(folder):
    distance_files = [f for f in os.listdir(folder) if f.startswith("distance_history_run_") and f.endswith(".csv")]
    best_distance = float("inf")
    best_file = None
    best_distances = []
    
    for file in distance_files:
        file_path = os.path.join(folder, file)
        df = pd.read_csv(file_path, header=None, names=["iteration", "distance"])
        min_distance = df["distance"].iloc[-1]  # 获取最终收敛的距离
        if min_distance < best_distance:
            best_distance = min_distance
            best_file = file_path
            best_distances = df

    return best_file, best_distances

# Step 3: 读取路径历史数据
def get_route_history(folder, distance_file):
    run_id = distance_file.split("_")[-1].split(".")[0]
    route_file = f"route_history_run_{run_id}.csv"
    route_path = os.path.join(folder, route_file)
    route_data = []
    with open(route_path, "r") as f:
        for line in f:
            points = line.strip().split(",")
            route = [tuple(map(float, p.split())) for p in points if p.strip()]
            route_data.append(route)
    return route_data

# Step 4: 绘制距离下降曲线
def plot_distance_curve(best_distances, folder):
    plt.figure(figsize=(8, 6))
    plt.plot(best_distances["iteration"], best_distances["distance"], label="Distance")
    plt.title("Distance Reduction Over Iterations")
    plt.xlabel("Iteration")
    plt.ylabel("Distance")
    plt.legend()
    plt.grid()
    # plt.show()
    plt.savefig(os.path.join(folder, "distance_curve.png"))

# Step 5: 绘制路径变化动画
def create_route_animation(route_data, folder):
    fig, ax = plt.subplots(figsize=(8, 6))
    scat, = ax.plot([], [], 'o-', lw=2)
    
    def init():
        ax.set_xlim(0, 100)
        ax.set_ylim(0, 100)
        ax.set_title("Route Optimization Animation")
        ax.set_xlabel("X Coordinate")
        ax.set_ylabel("Y Coordinate")
        return scat,

    def update(frame):
        points = route_data[frame]
        x, y = zip(*points)
        # connect the last point to the first point
        x = list(x) + [x[0]]
        y = list(y) + [y[0]]
        scat.set_data(x, y)
        ax.set_title(f"Iteration: {frame}")
        return scat,

    ani = FuncAnimation(fig, update, frames=len(route_data), init_func=init, blit=True)
    ani.save(os.path.join(folder, "route_animation.gif"), writer='pillow', fps=20)
    plt.close()

# 主程序
if __name__ == "__main__":
    # 获取有效的结果文件夹
    valid_folders = get_valid_folders(".")
    if not valid_folders:
        print("No valid result folders found.")
    else:
        for folder in valid_folders:
            print(f"Processing folder: {folder}")
            
            # 获取最佳运行的距离文件和数据
            best_file, best_distances = get_best_run(folder)
            if not best_file:
                print(f"No distance history files found in {folder}")
                continue

            # 读取路径数据
            route_data = get_route_history(folder, os.path.basename(best_file))
            
            # 绘制距离下降曲线
            print(f"Plotting distance curve for {folder}")
            plot_distance_curve(best_distances, folder)
            
            # 生成路径变化动画
            print(f"Creating animation for {folder}")
            create_route_animation(route_data, folder)