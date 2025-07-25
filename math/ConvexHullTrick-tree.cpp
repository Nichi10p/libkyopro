#include <cassert>
#include <utility>
#include <iterator>
#include <functional>
#include <set>

// https://www.slideshare.net/slideshow/convex-hull-trick/141103494
// https://yukicoder.me/submissions/910675
class Line {
  friend class CHT;
  static inline std::set<Line, std::greater<void>>::iterator _beg, _end;
  long long _a;
  long long mutable _b;
  std::set<Line, std::greater<void>>::iterator mutable _itr;
  public:
  Line(long long _a, long long _b) : _a(_a), _b(_b) {}
  // 傾きの降順
  friend bool operator>(Line const &_l, Line const &_r) {
    return _l._a > _r._a;
  }
  // 最小値クエリ処理
  friend bool operator>(Line const &_l, long long const _x) {
    if (_l._itr == _end || next(_l._itr) == _end)
      return false;
    auto const _nxt = next(_l._itr);
    auto const _i = _l._itr->_a * _x + _l._itr->_b;
    auto const _n = _nxt->_a    * _x + _nxt->_b;
    return _i > _n;
  }
  friend bool operator>(long long const _x, Line const &_l) {
    if (_l._itr == _beg)
      return true;
    return operator>(*prev(_l._itr), _x);
  }
};
class CHT {
  std::set<Line, std::greater<void>> _tree;
  // 直線の必要性判定
  // https://noshi91.hatenablog.com/entry/2021/03/23/200810
  auto _k(long long const _a, long long const _b, long long const _c, long long const _d) const {
    return (_d - _b) / (_a - _c);
  }
  bool _need(long long const _al, long long const _bl, long long const _am, long long const _bm, long long const _ar, long long const _br) const {
    // assert(_al > _am && _am > _ar);
    return _k(_al, _bl, _am, _bm) < _k(_am, _bm, _ar, _br);
  }
  // 不要な直線の削除
  void _erase_left(decltype(_tree)::iterator const _lr) {
    while (_lr != _tree.begin()) {
      auto _lm = prev(_lr);
      if (_lm == _tree.begin())
        return;
      auto _ll = prev(_lm);
      if (_need(_ll->_a, _ll->_b, _lm->_a, _lm->_b, _lr->_a, _lr->_b))
        return;
      _tree.erase(_lm);
    }
  }
  void _erase_right(decltype(_tree)::iterator const _ll) {
    while (_ll != _tree.end()) {
      auto _lm = next(_ll);
      if (_lm == _tree.end())
        return;
      auto _lr = next(_lm);
      if (_lr == _tree.end())
        return;
      if (_need(_ll->_a, _ll->_b, _lm->_a, _lm->_b, _lr->_a, _lr->_b))
        return;
      _tree.erase(_lm);
    }
  }
  public:
  CHT() {
    Line::_beg = _tree.begin();
    Line::_end = _tree.end();
  }
  // 追加クエリ (変更が発生したら true)
  bool insert(long long const _a, long long const _b) {
    // ベースケース
    if (_tree.empty()) {
      auto _node = _tree.emplace(_a, _b).first;
      _node->_itr = _node;
      return true;
    }
    // _a が最大
    if (_a > _tree.begin()->_a) {
      auto _ll = _tree.emplace_hint(_tree.begin(), _a, _b);
      _ll->_itr = _ll;
      _erase_right(_ll);
      return true;
    }
    // _a が最小
    if (_tree.rbegin()->_a > _a) {
      auto _lr = _tree.emplace_hint(_tree.end(), _a, _b);
      _lr->_itr = _lr;
      _erase_left(_lr);
      return true;
    }
    // 傾きが同一の直線が存在する
    Line _line(_a, _b);
    if (auto _lm=_tree.find(_line); _lm != _tree.end()) {
      if (_lm->_b <= _b)
        return false;
      _lm->_b = _b;
      _erase_left(_lm);
      _erase_right(_lm);
      return true;
    }
    // 直線 (_a, _b) が必要ない
    if (auto _lr=_tree.upper_bound(_line), _ll=prev(_lr); not _need(_ll->_a, _ll->_b, _a, _b, _lr->_a, _lr->_b))
      return false;
    // 直線 (_a, _b) を追加
    auto _lm = _tree.emplace(_a, _b).first;
    _lm->_itr = _lm;
    _erase_left(_lm);
    _erase_right(_lm);
    return true;
  }
  // 最小値クエリ
  auto find(long long const _x) const {
    assert(not _tree.empty());
    auto _itr = _tree.lower_bound(_x);
    return std::pair(_itr->_a, _itr->_b);
  }
};

// test
// https://atcoder.jp/contests/dp/submissions/67884921
