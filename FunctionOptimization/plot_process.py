import os
import glob
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Function to compute the Rastrigin function
def rastrigin(x, y):
    A = 10
    return A * 2 + (x**2 - A * np.cos(2 * np.pi * x)) + (y**2 - A * np.cos(2 * np.pi * y))

# Function to read all CSV files and find the best run for a specific data folder
def find_best_run(folder_path):
    csv_files = glob.glob(os.path.join(folder_path, "*.csv"))
    best_run = None
    best_fitness = float('inf')

    for csv_file in csv_files:
        data = pd.read_csv(csv_file)
        # Ensure the 'Fitness' column is numeric
        data['Fitness'] = pd.to_numeric(data['Fitness'], errors='coerce')

        # Drop rows with NaN Fitness values
        data = data.dropna(subset=['Fitness'])

        last_generation = data[data['Generation'] == data['Generation'].max()]
        min_fitness = last_generation['Fitness'].min()

        if min_fitness < best_fitness:
            best_fitness = min_fitness
            best_run = csv_file

    return best_run

# Function to generate the animation for the optimization process
def generate_animation(csv_file, output_path):
    data = pd.read_csv(csv_file)
    generations = data['Generation'].unique()

    # Create Rastrigin function background
    x = np.linspace(-5.12, 5.12, 200)
    y = np.linspace(-5.12, 5.12, 200)
    X, Y = np.meshgrid(x, y)
    Z = rastrigin(X, Y)

    fig, ax = plt.subplots()
    ax.contourf(X, Y, Z, levels=50, cmap='viridis')
    scatter = ax.scatter([], [], c='red', label='Population')
    ax.legend(loc='upper right')
    ax.set_title("Optimization Process")
    ax.set_xlim(-5.12, 5.12)
    ax.set_ylim(-5.12, 5.12)

    def update(gen):
        population = data[data['Generation'] == gen]
        scatter.set_offsets(population[['x1', 'x2']].values)
        ax.set_title(f"Generation: {gen}")
        return scatter,

    ani = FuncAnimation(fig, update, frames=generations, blit=True)
    ani.save(output_path, writer='pillow', fps=10)
    plt.close()

# Function to plot the fitness curves for all algorithms
def plot_fitness_curves(folder_paths, output_path):
    plt.figure(figsize=(10, 6))
    for folder_path in folder_paths:
        algorithm_name = os.path.basename(folder_path).replace("_data", "")
        csv_files = glob.glob(os.path.join(folder_path, "*.csv"))
        best_fitness_by_generation = []

        for csv_file in csv_files:
            data = pd.read_csv(csv_file)
            # Ensure 'Generation' and 'Fitness' columns are numeric
            data['Generation'] = pd.to_numeric(data['Generation'], errors='coerce')
            data['Fitness'] = pd.to_numeric(data['Fitness'], errors='coerce')

            # Drop rows with NaN in 'Generation' or 'Fitness'
            data = data.dropna(subset=['Generation', 'Fitness'])
            data['Generation'] = data['Generation'].astype(int)

            max_generation = data['Generation'].max()
            fitness_by_generation = [
                data[data['Generation'] == gen]['Fitness'].min() for gen in range(max_generation + 1)
            ]
            best_fitness_by_generation.append(fitness_by_generation)

        # Calculate the average fitness for each generation across runs
        best_fitness_by_generation = np.array(best_fitness_by_generation, dtype=float)  # Ensure array is numeric
        avg_fitness = np.mean(best_fitness_by_generation, axis=0)
        plt.plot(avg_fitness, label=algorithm_name)

    plt.xlabel("Generation")
    plt.ylabel("Best Fitness Value")
    plt.title("Fitness Improvement Over Generations")
    plt.legend()
    plt.savefig(output_path)
    plt.close()

# Main logic to find all *_data folders and process the files
current_dir = "."  # Change this to the directory containing your data folders if needed
data_folders = [os.path.join(current_dir, folder) for folder in os.listdir(current_dir) if folder.endswith("_data")]

# Paths for output files
output_fitness_curve_path = os.path.join(current_dir, "fitness_curves.png")

# Process each data folder to generate animations
# for folder in data_folders:
#     best_run_file = find_best_run(folder)
#     if best_run_file:
#         algorithm_name = os.path.basename(folder).replace("_data", "")
#         animation_output_path = os.path.join(current_dir, f"{algorithm_name}_optimization.gif")
#         generate_animation(best_run_file, animation_output_path)

# Plot fitness curves for all algorithms
plot_fitness_curves(data_folders, output_fitness_curve_path)