#include <random>
#include <functional>
#include <vector>
#include <iostream>
#include <chrono>
#include "algos.h"

template<class T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &data) {
  bool first = true;
  out << "[";
  for (const T &value: data) {
    if (!first) {
      out << ", ";
    }
    out << value;
    first = false;
  }
  out << "]";
  return out;
}

static const int ITERATIONS = 5;
static const int GRAPH_SIZE = 500;

vector<vector<int>> gen_graph(int n) {
  vector<vector<int>> g(n * n * n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        int v = i * n * n + j * n + k;
        if (i + 1 < n) {
          g[v].push_back((i + 1) * n * n + j * n + k);
        }
        if (j + 1 < n) {
          g[v].push_back(i * n * n + (j + 1) * n + k);
        }
        if (k + 1 < n) {
          g[v].push_back(i * n * n + j * n + (k + 1));
        }
      }
    }
  }
  return g;
}

vector<vector<int>> graph = gen_graph(GRAPH_SIZE);

std::vector<long> benchmark() {
  std::vector<long> iterationTimes(ITERATIONS);
  for (int iter = 0; iter < ITERATIONS; iter++) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    bfs(graph);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    iterationTimes[iter] = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Iteration " << (iter + 1) << " took " << iterationTimes[iter] << " ms" << std::endl;
  }
  std::sort(iterationTimes.begin(), iterationTimes.end());
  return iterationTimes;
}

std::vector<long> benchmark_par() {
  std::vector<long> iterationTimes(ITERATIONS);

  for (int iter = 0; iter < ITERATIONS; iter++) {
    double begin = omp_get_wtime();

    bfs_par(graph);

    double end = omp_get_wtime();
    iterationTimes[iter] = (long) ((end - begin) * 1000);
    std::cout << "Iteration " << (iter + 1) << " took " << iterationTimes[iter] << " ms" << std::endl;
  }
  std::sort(iterationTimes.begin(), iterationTimes.end());
  return iterationTimes;
}

void prettifyStatistics(std::vector<long> statistics) {
  std::cout.precision(4);
  std::cout << std::fixed;
  std::cout << "All iterations in ms " << statistics << std::endl;
  std::cout << "Max time " << *std::max_element(statistics.begin(), statistics.end()) << " ms" << std::endl;
  std::cout << "Min time " << *std::min_element(statistics.begin(), statistics.end()) << " ms" << std::endl;
  std::cout << "Mean time " << std::accumulate(statistics.begin(), statistics.end(), 0l) / double(statistics.size())
            << " ms" << std::endl;
  std::cout << std::endl;
}

int main() {
  omp_set_dynamic(1);
  omp_set_num_threads(4);

  std::cout << "Parallel algo" << std::endl;
  prettifyStatistics(benchmark_par());

  std::cout << "Sequential algo" << std::endl;
  prettifyStatistics(benchmark());
  return 0;
}
