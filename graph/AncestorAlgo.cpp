#include <cassert>
#include <bit>
#include <vector>
using namespace std;

class AncestorAlgo {
  private:
  int w, n;
  vector<int> par, dep;
  constexpr int const& _par(int const e, int const v) const {
    assert(0 <= e && e < w);
    assert(0 <= v && v < n);
    return par[e*n + v];
  }
  constexpr void build_par() {
    for (int e{1}; e < w; ++e)
      for (int v{0}; v < n; ++v)
        if (0 <= _par(e-1, v))
          const_cast<int&>(_par(e, v)) = _par(e-1, _par(e-1, v));
        else
          const_cast<int&>(_par(e, v)) = -1;
  }
  public:
  constexpr AncestorAlgo(int const root, vector<vector<int>> const &graph)
  : w(bit_width(graph.size())), n(graph.size()), par(w*n, -1), dep(n)
  {
    vector<int> dfs = {root};
    while (!dfs.empty()) {
      int const u{dfs.back()};
      dfs.pop_back();
      for (int const v : graph[u])
        if (v != par[u]) {
          par[v] = u;
          dep[v] = dep[u] + 1;
          dfs.push_back(v);
        }
    }
    build_par();
  }
  constexpr explicit AncestorAlgo(vector<int> const &parent_id)
  : w(bit_width(parent_id.size())), n(parent_id.size()), par(w*n, -1), dep(n, -1)
  {
    vector<int> dfs;
    for (int _v{0}; _v < n; ++_v) {
      dfs.push_back(_v);
      while (0 <= parent_id[dfs.back()] && dep[dfs.back()] < 0)
        dfs.push_back(parent_id[dfs.back()]);
      int p = dfs.back();
      dfs.pop_back();
      if (dep[p] < 0)
        dep[p] = 0;
      while (!dfs.empty()) {
        par[dfs.back()] = p;
        dep[dfs.back()] = dep[p] + 1;
        p = dfs.back();
        dfs.pop_back();
      }
    }
    build_par();
  }
  // Level Ancestor
  constexpr int LA(int v, int d) const {
    if (dep[v] < d)
      return -1;
    unsigned k = dep[v] - d;
    while (k) {
      v = _par(countr_zero(k), v);
      k ^= 1 << countr_zero(k);
    }
    return v;
  }
  // Lowest Common Ancestor
  constexpr int LCA(int v1, int v2) const {
    if (dep[v1] < dep[v2])
      v2 = LA(v2, dep[v1]);
    else
      v1 = LA(v1, dep[v2]);
    if (v1 == v2)
      return v1;
    for (int e = bit_width<unsigned>(dep[v1]); e--; )
      if (_par(e, v1) != _par(e, v2))
        v1 = _par(e, v1), v2 = _par(e, v2);
    return par[v1];
  }
};
