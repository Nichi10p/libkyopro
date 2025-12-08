// AtCoder Beginner Contest 435 E
// https://atcoder.jp/contests/abc435/tasks/abc435_e
// https://atcoder.jp/contests/abc435/submissions/71566033
#include <cstdio>
#include <map>
#include <algorithm>

struct interval_set {
  std::map<int64_t,int64_t> _map;
  int64_t erase(int64_t const &_l, int64_t const &_r) {
    int64_t _cnt = 0;
    auto _itr = _map.upper_bound(_l);
    while (_itr != _map.end() && _itr->second < _r) {
      // _itr の左端が残る場合
      if (_itr->second < _l)
        _map.emplace_hint(_itr, _l, _itr->second);
      // _itr の右端が残る場合
      if (_r < _itr->first) {
        _cnt += _r - std::max(_l, _itr->second);
        _itr->second = _r;
        ++_itr;
      }
      // _itr の右端が消える場合
      else {
        _cnt += _itr->first - std::max(_l, _itr->second);
        _map.erase(_itr++);
      }
    }
    return _cnt;
  }
  int64_t insert(int64_t const &_l, int64_t const &_r) {
    int64_t _cnt = _r - _l;
    _cnt -= erase(_l, _r);
    _map.emplace(_r, _l);
    return _cnt;
  }
};

int main() {
  int n, q;
  scanf("%d %d ", &n, &q);

  int ans = n;
  interval_set st;
  while (q--) {
    int l, r;
    scanf("%d %d ", &l, &r);
    ans -= st.insert(l, r+1);
    printf("%d\n", ans);
  }
}
