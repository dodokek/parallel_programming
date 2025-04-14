#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>

std::mutex mtx;

// Partition function
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) std::swap(arr[++i], arr[j]);
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Parallel quicksort function
void parallel_quicksort(std::vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        if (depth < 4) {
            std::thread t1([&]() { parallel_quicksort(arr, low, pivot - 1, depth + 1); });
            parallel_quicksort(arr, pivot + 1, high, depth + 1);
            t1.join();
        } else {
            parallel_quicksort(arr, low, pivot - 1, depth + 1);
            parallel_quicksort(arr, pivot + 1, high, depth + 1);
        }
    }
}

// Generates a vector with random integers
std::vector<int> generate_random_vector(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000000);
    std::vector<int> vec(size);
    for (auto& v : vec) v = dist(gen);
    return vec;
}

// Measures the time taken by a function
template<typename Func>
long long measure_time(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end   = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    std::ofstream outfile("quicksort_comparison.csv");
    outfile << "ArraySize,SequentialTime(ms),ParallelTime(ms)\n";

    for (size_t size : {1000, 10000, 50000, 100000, 500000, 1000000}) {
        auto base = generate_random_vector(size);
        auto copy1 = base;
        auto copy2 = base;

        long long seq_time = measure_time([&]() {
            std::sort(copy1.begin(), copy1.end());
        });

        long long par_time = measure_time([&]() {
            parallel_quicksort(copy2, 0, copy2.size() - 1);
        });

        std::cout << "Size: " << size
                  << ", Sequential: " << seq_time << "ms"
                  << ", Parallel: " << par_time << "ms\n";

        outfile << size << "," << seq_time << "," << par_time << "\n";
    }

    outfile.close();
    return 0;
}
