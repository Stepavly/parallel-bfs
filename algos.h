//
// Created by stepavly on 20.11.2022.
//

#ifndef QUICKSORT__ALGOS_H_
#define QUICKSORT__ALGOS_H_

#include <type_traits>
#include <iterator>
#include <iostream>
#include <queue>
#include "omp.h"
#include <atomic>
#include <memory>
#include <cassert>

using std::vector;
using std::atomic;
using std::shared_ptr;
using std::pair;
using std::queue;

struct vertex {
    int x, y, z;
};

std::ostream &operator<<(std::ostream &out, const vertex &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

vector<int> bfs(const vector<vector<int>>& g) {
  queue<int> q;
  vector<int> dist(g.size(), -1);
  dist[0] = 0;
  q.push(0);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int u: g[v]) {
      if (dist[u] == -1) {
        dist[u] = dist[v] + 1;
        q.push(u);
      }
    }
  }

  return std::move(dist);
}

vector<int> bfs_par(const vector<vector<int>>& g) {
  vector<int> q;
  vector<int> dist(g.size(), -1);
  dist[0] = 0;
  q.push_back(0);

  vector<int> deg(1);
  vector<int> next_front;
  while (!q.empty()) {
    deg.resize(q.size());
    int sum_deg = 0;
#pragma omp parallel for simd reduction(inscan, +: sum_deg)
    for (size_t i = 0; i < q.size(); i++) {
      deg[i] = sum_deg;
#pragma omp scan exclusive(sum_deg)
      sum_deg += g[q[i]].size();
    }
    next_front.resize(sum_deg, -1);

    if (sum_deg == 0) {
      break;
    }

#pragma omp parallel for
    for (size_t i = 0; i < sum_deg; i++) {
      next_front[i] = -1;
    }

#pragma omp parallel for shared(dist)
    for (size_t i = 0; i < q.size(); i++) {
      int v = q[i];
      if (v == -1) {
        continue;
      }

      int next_dist = dist[v] + 1, next_front_pos = deg[i];
      for (int u: g[v]) {
        int expected_dist = -1;
#pragma omp atomic compare capture
        if (dist[u] == -1) {
          dist[u] = next_dist;
        } else {
          expected_dist = dist[u];
        }
        if (expected_dist == -1) {
          next_front[next_front_pos] = u;
        }
        next_front_pos++;
      }
    }

    q.clear();
    for (int i: next_front) {
      if (i != -1) {
        q.push_back(i);
      }
    }
  }
  return std::move(dist);
}

#endif //QUICKSORT__ALGOS_H_
