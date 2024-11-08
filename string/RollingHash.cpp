#include <cstddef>
#include <cassert>
#include <random>
#include <vector>
#include <string_view>
using namespace std;

// qiita.com/keymoon/items/11fac5627672a6d6a9f6
// www.slideshare.net/slideshow/rolling-hash-149990902/149990902
class RollingHash {
  private:
  struct Mod {
    static constexpr size_t value() {
      return (static_cast<size_t>(1) << 61) - 1;
    }
    static constexpr size_t mul(size_t const a, size_t const b) {
      assert(a < value());
      assert(b < value());
      constexpr size_t mask31{(static_cast<size_t>(1) << 31) - 1};
      constexpr size_t mask30{(static_cast<size_t>(1) << 30) - 1};
      size_t const au{a >> 31}, ad{a & mask31};
      size_t const bu{b >> 31}, bd{b & mask31};
      size_t const m{ad*bu + au*bd};
      size_t const mu{m >> 30}, md{m & mask30};
      return au*bu*2 + mu+(md<<31) + ad*bd;
    }
    friend constexpr size_t operator%(size_t h, Mod) {
      h = (h >> 61) + (h & value());
      if (h >= value())
        h -= value();
      return h;
    }
  };
  static inline size_t _base{0};
  static inline vector<size_t> _powb;  // pow(base, i) % mod
  static void _extend_powb(size_t const new_size) {
    while (_powb.size() < new_size)
      _powb.push_back(Mod::mul(_powb.back(), _base) % Mod{});
  }
  vector<size_t> _hash;  // hash of str[0:i)
  public:
  class HashInfo {
    private:
    size_t _len, _val;
    public:
    HashInfo(size_t const len, size_t const val) : _len(len), _val(val) {}
    HashInfo& operator+=(HashInfo const &h) {
      _len += h._len;
      _val = (Mod::mul(_val, _powb[h._len]) + h._val) % Mod{};
      return *this;
    }
    friend HashInfo operator+(HashInfo h1, HashInfo const &h2) {
      h1 += h2;
      return h1;
    }
    friend bool operator==(HashInfo const &h1, HashInfo const &h2) {
      return h1._len == h2._len && h1._val == h2._val;
    }
  };
  static void set_base() {
    assert(_base == 0);
    random_device dev;
    uniform_int_distribution<size_t> randint{2, Mod::value()-2};
    _base = randint(dev);
    _powb = {1};
  }
  static HashInfo hash(string_view const str) {
    assert(_base > 1);
    size_t h{0};
    for (char const c : str)
      h = (Mod::mul(h, _base) + c) % Mod{};
    return HashInfo{str.size(), h};
  }
  RollingHash() = default;
  explicit RollingHash(string_view const str) : _hash(str.size()+1, 0) {
    assert(_base > 1);
    for (size_t i{0}; i < str.size(); ++i)
      _hash[i+1] = (Mod::mul(_hash[i], _base) + str[i]) % Mod{};
    _extend_powb(_hash.size());
  }
  HashInfo substr(size_t const pos, size_t const len) const {
    assert(pos+len < _hash.size());
    constexpr size_t positivizer{Mod::value() * 4};
    return HashInfo{len, (_hash[pos+len] + positivizer - Mod::mul(_hash[pos], _powb[len])) % Mod{}};
  }
  vector<size_t> search(string_view const str) const {
    HashInfo h{hash(str)};
    vector<size_t> idx;
    for (size_t i{0}; i+str.size() < _hash.size(); ++i)
      if (substr(i, str.size()) == h)
        idx.push_back(i);
    return idx;
  }
  RollingHash& operator+=(RollingHash const &rh) {
    size_t const _this{_hash.back()};
    _hash.reserve(_hash.size() + rh._hash.size());
    for (size_t i{1}; i < rh._hash.size(); ++i)
      _hash.push_back((Mod::mul(_this, _powb[i]) + rh._hash[i]) % Mod{});
    _extend_powb(_hash.size());
    return *this;
  }
  friend RollingHash operator+(RollingHash rh1, RollingHash const &rh2) {
    rh1 += rh2;
    return rh1;
  }
};
