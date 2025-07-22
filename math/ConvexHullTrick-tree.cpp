#include <cassert>
#include <utility>
#include <iterator>
#include <functional>
#include <numeric>
#include "ext/pb_ds/assoc_container.hpp"
typedef long long ll;

// https://www.slideshare.net/slideshow/convex-hull-trick/141103494
class CHT {
  __gnu_pbds::tree<ll, ll, std::greater<ll>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>
    _tree;
  // 直線の必要性判定
  // https://noshi91.hatenablog.com/entry/2021/03/23/200810
  ll _k(ll const _a, ll const _b, ll const _c, ll const _d) const {
    return (_d - _b) / (_a - _c);
  }
  bool _need1(ll const _a0, ll const _b0, ll const _a1, ll const _b1, ll const _a2, ll const _b2) const {
    assert(_a0 > _a1 && _a1 > _a2);
    return _k(_a0, _b0, _a1, _b1) < _k(_a1, _b1, _a2, _b2);
  }
  // 不要な直線の削除
  void _erase_left(decltype(_tree)::iterator const _l2) {
    assert(_l2 != _tree.begin());
    while (1) {
      auto _l1 = prev(_l2);
      if (_l1 == _tree.begin())
        return;
      auto _l0 = prev(_l1);
      if (_need1(_l0->first, _l0->second, _l1->first, _l1->second, _l2->first, _l2->second))
        return;
      _tree.erase(_l1);
    }
  }
  void _erase_right(decltype(_tree)::iterator const _l0) {
    assert(_l0 != _tree.end());
    while (1) {
      auto _l1 = next(_l0);
      if (_l1 == _tree.end())
        return;
      auto _l2 = next(_l1);
      if (_l2 == _tree.end())
        return;
      if (_need1(_l0->first, _l0->second, _l1->first, _l1->second, _l2->first, _l2->second))
        return;
      _tree.erase(_l1);
    }
  }
  public:
  CHT() = default;
  // 追加クエリ (変更が発生したら true )
  bool insert(ll const _a, ll const _b) {
    // ベースケース
    if (_tree.size() == 0) {
      _tree[_a] = _b;
      return true;
    }
    if (_tree.size() == 1) {
      if (_tree.begin()->first == _a && _tree.begin()->second < _b)
        return false;
      _tree[_a] = _b;
      return true;
    }
    // _a が最大
    if (_a > _tree.begin()->first) {
      auto _l0 = _tree.insert({_a, _b}).first;
      _erase_right(_l0);
      return true;
    }
    // _a が最小
    if (_tree.rbegin()->first > _a) {
      auto _l2 = _tree.insert({_a, _b}).first;
      _erase_left(_l2);
      return true;
    }
    // 傾きが同一の直線が存在する
    if (auto _l1=_tree.find(_a); _l1 != _tree.end()) {
      if (_l1->second <= _b)
        return false;
      _l1->second = _b;
      _erase_left(_l1);
      _erase_right(_l1);
      return true;
    }
    // 直線 (_a, _b) が必要ない
    if (auto _l2=_tree.upper_bound(_a), _l0=prev(_l2); not _need1(_l0->first, _l0->second, _a, _b, _l2->first, _l2->second))
      return false;
    // 直線 (_a, _b) を追加
    auto _l1 = _tree.insert({_a, _b}).first;
    _erase_left(_l1);
    _erase_right(_l1);
    return true;
  }
  // 最小値クエリ
  auto find(ll const x) const {
    assert(not _tree.empty());
    size_t _lo=0, _hi=_tree.size()-1;
    while (_lo < _hi) {
      size_t const _m = std::midpoint(_lo, _hi);
      auto _itr = _tree.find_by_order(_m);
      auto _jtr = _tree.find_by_order(_m + 1);
      if (_itr->first * x + _itr->second < _jtr->first * x + _jtr->second)
        _hi = _m;
      else
        _lo = _m + 1;
    }
    auto _itr = _tree.find_by_order(_lo);
    return std::pair(_itr->first, _itr->second);
  }
};

// test
// https://atcoder.jp/contests/dp/submissions/67839379
