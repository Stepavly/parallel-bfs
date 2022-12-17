//
// Created by stepavly on 27.11.2022.
//

#include <random>
#include <functional>
#include <vector>
#include <iostream>
#include "algos.h"

using std::vector;
using std::cout, std::endl;

static const int N = 100;

void check(vector<int> dist) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        assert(dist[i * N * N + j * N + k] == i + j + k);
      }
    }
  }
}

void check(vector<atomic<int>> dist) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        assert(dist[i * N * N + j * N + k] == i + j + k);
      }
    }
  }
}

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

int main() {
  check(bfs(gen_graph(N)));
  check(bfs_par(gen_graph(N)));
}
