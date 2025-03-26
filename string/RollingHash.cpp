#include <random>
#include <vector>
#include <string_view>
using namespace std;

// qiita.com/keymoon/items/11fac5627672a6d6a9f6
// www.slideshare.net/slideshow/rolling-hash-149990902/149990902
using uint128_t = __uint128_t;
// using boost::multiprecision::uint128_t;
class RollingHash {
  private:
  struct _mod {
    static constexpr size_t _value = (1ULL << 61) - 1;
    friend constexpr size_t operator%(uint128_t _n, _mod) {
      _n = (_n >> 61) + (_n & _value);
      if (_n >= _value)
        _n -= _value;
      return _n;
    }
  };
  static inline uint128_t      _base{0};
  static inline vector<size_t> _powb;  // pow(base, i) % mod
  vector<size_t>               _hash;  // hash(str[0:i))
  static void _set_base() {
    random_device _dev;
    uniform_int_distribution<size_t> _randint{2, _mod::_value-2};
    _base = _randint(_dev);
    _powb.assign(1, 1);
  }
  static void _extend_powb(size_t const _n) {
    if (_powb.size() >= _n)
      return;
    size_t _i = _powb.size();
    size_t _v = _powb.back();
    _powb.resize(_n);
    while (_i < _n) {
      _v = _v * _base % _mod{};
      _powb[_i++] = _v;
    }
  }
  static auto _full_hash(string_view const _sv) {
    size_t _h{0};
    for (char const _c : _sv)
      _h = (_h * _base + _c) % _mod{};
    return _h;
  }
  public:
  RollingHash() = default;
  explicit RollingHash(string_view const _sv) : _hash(_sv.size()+1) {
    if (_powb.empty())
      _set_base();
    for (size_t _i=0; _i < _sv.size(); ++_i)
      _hash[_i+1] = (_hash[_i] * _base + _sv[_i]) % _mod{};
    _extend_powb(_hash.size());
  }
  auto substr_hash(size_t const _pos = 0, size_t _len = -1) const {
    size_t _hl, _hr;
    if (_len == (size_t)-1 || _pos + _len >= _hash.size()) {
      // _pos + _len == _hash.size()-1
      _hr = _hash.back();
      _hl = uint128_t{_hash[_pos]} * _powb[_hash.size()-1 - _pos] % _mod{};
    }
    else {
      _hr = _hash[_pos+_len];
      _hl = uint128_t{_hash[_pos]} * _powb[_len] % _mod{};
    }
    if (_hl <= _hr)
      return _hr - _hl;
    else
      return _hr + _mod::_value - _hl;
  }
  bool starts_with(string_view _sv) const {
    return _sv.size() < _hash.size() && substr_hash(0, _sv.size()) == _full_hash(_sv);
  }
  bool ends_with(string_view _sv) const {
    return _sv.size() < _hash.size() && substr_hash(_hash.size()-1 - _sv.size()) == _full_hash(_sv);
  }
  auto find(string_view _sv, size_t const _pos = 0) const {
    auto const _h = _full_hash(_sv);
    for (size_t _i=_pos; _i+_sv.size() < _hash.size(); ++_i)
      if (substr_hash(_i, _sv.size()) == _h)
        return _i;
    return (size_t)-1;
  }
  auto rfind(string_view _sv, size_t const _pos = -1) const {
    if (_hash.size() < _sv.size())
      return (size_t)-1;
    auto const _h = _full_hash(_sv);
    size_t _i;
    if (_pos >= _hash.size())
      _i = _hash.size() - _sv.size();
    else
      _i = _pos + 1;
    while (_i--)
      if (substr_hash(_i, _sv.size()) == _h)
        return _i;
    return (size_t)-1;
  }
  void push_back(char const _c) {
    _hash.push_back((_hash.back() * _base + _c) % _mod{});
    _extend_powb(_hash.size());
  }
  RollingHash& operator+=(char const _c) {
    push_back(_c);
    return *this;
  }
  RollingHash& operator+=(string_view _sv) {
    size_t const _z = _hash.size();
    _hash.resize(_hash.size() + _sv.size());
    for (size_t _i=0; _i < _sv.size(); ++_i)
      _hash[_z+_i] = (_hash[_z+_i-1] * _base + _sv[_i]) % _mod{};
    _extend_powb(_hash.size());
    return *this;
  }
  RollingHash& operator+=(RollingHash const &_rh) {
    size_t const    _z = _hash.size();
    uint128_t const _h = _hash.back();
    _hash.resize(_hash.size() + _rh._hash.size() - 1);
    for (size_t _i=1; _i < _rh._hash.size(); ++_i)
      _hash[_z+_i-1] = (_h * _powb[_i] + _rh._hash[_i]) % _mod{};
    _extend_powb(_hash.size());
    return *this;
  }
};
