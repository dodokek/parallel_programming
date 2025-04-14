import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("quicksort_comparison.csv")

plt.plot(df["ArraySize"], df["SequentialTime(ms)"], label="Sequential std::sort")
plt.plot(df["ArraySize"], df["ParallelTime(ms)"], label="Parallel QuickSort")
plt.xlabel("Array Size")
plt.ylabel("Execution Time (ms)")
plt.title("Sequential vs Parallel QuickSort Performance")
plt.legend()
plt.grid(True)
plt.show()
