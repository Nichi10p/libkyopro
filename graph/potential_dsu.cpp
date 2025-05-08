#include <concepts>
#include <utility>
#include <vector>
#include <algorithm>
#include <ranges>
using namespace std;

namespace Nichi10p {
// cf. github.com/atcoder/ac-library/blob/master/atcoder/dsu.hpp
// cf. github.com/noshi91/NoshiMochiLibrary/blob/master/Tree/UnionFindTree/PotentializedUnionFind.noshi.cpp
// cf. qiita.com/drken/items/cce6fc5c579051e64fab
template<typename _type> concept Abel = requires(_type _a, _type _b) {
  { _a =  _b } -> same_as<_type&>;
  { _a += _b } -> same_as<_type&>;
  { _a -= _b } -> same_as<_type&>;
  { _a + _b } -> same_as<_type>;
  { _a - _b } -> same_as<_type>;
};
template<Abel _type=int, _type _unit=0> class potential_dsu {
  // `_node[i].first`  = if `i` is root; then -(component size); else (parent index);
  // `_node[i].second` = (potential)
  vector<pair<int,_type>> _node;
  pair<int,_type> _find(int _v) /* -> leader, potential */ {
    _type _p{_unit};
    // path halving
    while (_node[_v].first >= 0 && _node[_node[_v].first].first >= 0) {
      auto &_now=_node[_v], &_par=_node[_node[_v].first];
      _p += _now.second += _par.second;
      _v =  _now.first  =  _par.first;
    }
    if (_node[_v].first >= 0) {
      _p += _node[_v].second;
      _v =  _node[_v].first;
    }
    return {_v, _p};
  }
  public:
  explicit potential_dsu(int const _n) : _node(_n, {-1,_unit}) {}
  // potential(v) == potential(u) + d
  bool merge(int _u, int _v, _type _d=_unit) {
    auto const [_lu, _pu] = _find(_u);
    auto const [_lv, _pv] = _find(_v);
    _u = _lu;
    _v = _lv;
    _d = (_pu + _d) - _pv;
    if (_u == _v)
      return false;
    if (-_node[_u].first < -_node[_v].first) {
      std::ranges::swap(_u, _v);
      _d = _unit - _d;
    }
    _node[_u].first += _node[_v].first;
    _node[_v].first  = _u;
    _node[_v].second = _d;
    return true;
  }
  bool same(int const _u, int const _v) {
    return leader(_u) == leader(_v);
  }
  int leader(int const _v) {
    return _find(_v).first;
  }
  int size(int const _v) {
    return -_node[leader(_v)].first;
  }
  vector<vector<int>> groups() {
    vector<vector<int>> _ret(_node.size());
    for (int const _i : std::views::iota(0, std::ranges::ssize(_node)))
      _ret[leader(_i)].push_back(_i);
    std::erase_if(_ret, std::ranges::empty);
    return _ret;
  }
  // potential(v) - potential(u)
  _type diff(int const _u, int const _v) {
    return _find(_v).second - _find(_u).second;
  }
};
}

// use sample
// ABC328F Good Set Query
// atcoder.jp/contests/abc328/tasks/abc328_f
#include <iostream>
typedef long long ll;

int main() {
  int n, q;
  cin >> n >> q;

  vector<int> ans;
  Nichi10p::potential_dsu<ll> G(n);
  for (int const i : std::views::iota(1) | std::views::take(q)) {
    int a, b, d;
    cin >> a >> b >> d;
    --a; --b;
    if (G.same(a, b)) {
      if (G.diff(b, a) == d)
        ans.push_back(i);
    }
    else {
      G.merge(b, a, d);
      ans.push_back(i);
    }
  }

  for (size_t i=0; i < ans.size(); ++i)
    cout << ans[i] << " \n"[i+1==ans.size()];
}
