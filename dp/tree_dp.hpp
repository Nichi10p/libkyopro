#include <vector>


namespace Nichi10p {

struct DATA { int dsum, size; };
DATA e() { return {0, 0}; }
DATA merge(const DATA &a, const DATA &b) { return {a.dsum+b.dsum, a.size+b.size}; }
DATA up(const DATA &x) { return {x.dsum+x.size+1, x.size+1}; }

class Solver {
public:
  Solver() {}
  void add_edge(const int &u, const int &v) {
    _g[u].push_back(v);
    _g[v].push_back(u);
  }
  void dp(const int &root) {
    _root = root;
    _total[root] = _dp(root, -1);
  }
  void rerooting() {}
private:
  std::vector<std::vector<int>> _g;
  std::vector<std::vector<DATA>> _sub;  // 頂点 i の j 番目の辺に対応する部分木のdp
  std::vector<DATA> _total;             // 頂点 i を根とした木のdp
  int _root;
  DATA _dp(const int &u, const int &p) {
    DATA data{e()};
    _sub[u].resize(_g[u].size());
    for (unsigned i = 0; i < _g[u].size(); ++i)
      if (_g[u][i] != p) {
        _sub[u][i];
      }
  }
};

}