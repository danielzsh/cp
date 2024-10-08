#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include "constants.h"
#include "util.h"
using namespace std;

struct W { // weighted edge
  int v; ll w;
  friend istream& operator>>(istream& is, W &e) { return is >> e.v >> e.w; }
  friend ostream& operator<<(ostream& os, W &e) { return os << e.v << ":" << e.w; }
  bool operator==(const W o) const { return make_pair(v, w) == make_pair(o.v, o.w); }
  operator int() const { return v; }
  bool operator<(const W &o) const { return make_pair(v, w) < make_pair(o.v, o.w); }
  bool operator<(int x) const { return v < x; }
};

template <int N, typename T = int, class V = vector<T>>
struct Graph {
  int n, m;
  Graph(int n, int m) : n(n), m(m) {}
  V g[N];
  void add(int u, T e) { ::add(g[u], e); }
  void u(int u, int v) {
    if constexpr (is_same_v<T, int>) {
      add(u, v);
      add(v, u);
    }
  }
  // undirected input
  Graph& input() {
    for (int i = 0; i < m; i++) {
      int u; T v; cin >> u >> v;
      ::add(g[u], v);
      if constexpr (is_same_v<T, W>) ::add(g[v], W{u, v.w});
      else ::add(g[v], u);
    }
    return *this;
  }
};

#ifdef LCA
#define EULER
#include "sparse.h"
#endif

template <int N, typename T = int, typename V = vector<T>>
struct Tree : Graph<N, T, V> {
  int r, s[N], p[N], d[N]{};
  Tree(int n) : Graph<N, T, V>(n, n - 1) {
    fill(s, s + n + 1, 1); // TODO: allow initializing weighted nodes later
  }
  Tree& root(int x) { return r = p[x] = x, *this; }
  #ifdef EULER
  int I[N], o[2 * N], t = 0;
  ST<2 * N> st{[&](int x, int y) { return d[x] < d[y] ? x : y; }}; 
  #endif
  int dfs(int x) {
    for (T e : this->g[x]) {
      #ifdef EULER
      o[I[x] = t++] = x;
      #endif
      if (e == p[x]) continue;
      int w = 1;
      if constexpr (is_same_v<T, W>) w = e.w;
      d[e] = d[p[e] = x] + w;
      s[x] += dfs(e);
    }
    #ifdef LCA
    if (x == r) st.build(o);
    #endif
    return s[x];
  }
  #ifdef LCA
  int lca(int u, int v) {
    if (I[u] > I[v]) swap(u, v);
    return st.F(I[u], I[v] + 1);
  }
  int D(int u, int v) {
    return d[u] + d[v] - 2 * d[lca(u, v)];
  }
  #endif
  #define CENTROID
  #ifdef CENTROID
  int cr;
  vector<int> cg[N];
  int decompose(int x, int n) {
    for (int y : this->g[x]) {
      if (s[y] > n / 2) return decompose(y, n);
    }
    for(int y : this->g[x]) {
      W e = *this->g[y].find(x);
      this->g[y].erase(e);
      add(cg[x], decompose(y, s[y]));
      add(this->g[y], e);
    }
    return x;
  }
  Tree& decompose() { return cr = decompose(r, s[r]), *this; }
  #endif

  Tree& dfs() { return dfs(r), *this; }
  Tree& input() { return Graph<N, T, V>::input(), *this; }
};
