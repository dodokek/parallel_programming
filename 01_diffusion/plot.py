import numpy as np
import matplotlib.pyplot as plt

# Path to your file
filename = 'dump_grid.txt'

# Read and parse the grid
grid = []
with open(filename, 'r') as f:
    for line in f:
        # Split on '|' and remove empty entries and whitespace
        row = [float(cell.strip()) for cell in line.strip().split('|') if cell.strip()]
        if row:
            grid.append(row)

# Convert to NumPy array
Z = np.array(grid)
rows, cols = Z.shape

# Create X and Y coordinate grids
X, Y = np.meshgrid(np.arange(cols), np.arange(rows))

# Plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the surface
ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='k')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z (value in grid cell)')
plt.title('3D Grid Surface Plot')

plt.show()