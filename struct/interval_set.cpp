// C++11
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
