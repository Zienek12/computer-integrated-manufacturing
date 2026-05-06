import sys
import re
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import subprocess 
def main():
    nodes = {}
    history = []

    
    exe_path = r"../out/build/x64-release/SA/SA.exe"  
    
    try:
        print(f"Activating C++ app: {exe_path}...")
        result = subprocess.run([exe_path], capture_output=True, text=True, check=True)
        input_data = result.stdout.splitlines()
        print("Successfully retrieved data from C++. Generating visualization...")
    except FileNotFoundError:
        print(f"Error: File not found {exe_path}.")
        print("Make sure the project is built and the path to the .exe is correct.")
        return
    except subprocess.CalledProcessError as e:
        print(f"Error while executing C++: {e}")
        return

    for line in input_data:
        node_match = re.match(r"Node (\d+): \((\d+), (\d+)\)", line)
        if node_match:
            node_id = int(node_match.group(1))
            x = int(node_match.group(2))
            y = int(node_match.group(3))
            nodes[node_id] = (x, y)
            continue
        
        step_match = re.match(r"STEP (\d+) DIST (\d+) PATH (.*)", line)
        if step_match:
            step = int(step_match.group(1))
            dist = int(step_match.group(2))
            path_str = step_match.group(3)
            path = [int(p) for p in path_str.strip().split()]
            history.append((step, dist, path))

    if not nodes or not history:
        print("No valid data for visualization.")
        return

    fig, ax = plt.subplots(figsize=(10, 6))
    
    all_x = [pos[0] for pos in nodes.values()]
    all_y = [pos[1] for pos in nodes.values()]
    
    ax.scatter(all_x, all_y, color='crimson', zorder=5)
    ax.scatter(nodes[0][0], nodes[0][1], color='lime', s=120, edgecolors='black', zorder=6, label='Sklep / Baza')
    line_plot, = ax.plot([], [], linestyle='-', color='dodgerblue', alpha=0.7)
    title_text = ax.set_title("")

    ax.set_xlim(min(all_x) - 10, max(all_x) + 10)
    ax.set_ylim(min(all_y) - 10, max(all_y) + 10)
    ax.grid(True, linestyle='--', alpha=0.5)

    def init():
        line_plot.set_data([], [])
        title_text.set_text("")
        return line_plot, title_text

    def update(frame):
        step, dist, path = history[frame]
        x_coords = [nodes[node_id][0] for node_id in path]
        y_coords = [nodes[node_id][1] for node_id in path]
        
        line_plot.set_data(x_coords, y_coords)
        title_text.set_text(f"Simulated Annealing: Step {step} | Best Distance: {dist}")
        return line_plot, title_text

    anim = animation.FuncAnimation(
        fig, update, frames=len(history), init_func=init,
        interval=25, blit=True, repeat=False
    )

    plt.show()

if __name__ == "__main__":
    main()