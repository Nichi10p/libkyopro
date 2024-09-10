#include <cassert>
#include <random>
#include <vector>
#include <string_view>
using namespace std;

// qiita.com/keymoon/items/11fac5627672a6d6a9f6
class RollingHash {
  private:
  struct Mod {
    static constexpr size_t value() {
      return (static_cast<size_t>(1) << 61) - 1;
    }
    friend constexpr size_t operator%(size_t h, Mod) {
      h = (h >> 61) + (h & Mod::value());
      if (h >= Mod::value())
        h -= Mod::value();
      return h;
    }
  };
  struct Base {
    static size_t value() {
      static random_device                    dev;
      static uniform_int_distribution<size_t> randint{128, Mod::value()-1};
      static size_t const _value{randint(dev)};
      return _value;
    }
  };
  static constexpr size_t _mul_(size_t const a, size_t const b) {
    assert(a < Mod::value());
    assert(b < Mod::value());
    constexpr size_t mask31{(static_cast<size_t>(1) << 31) - 1};
    constexpr size_t mask30{(static_cast<size_t>(1) << 30) - 1};
    size_t const au{a >> 31}, ad{a & mask31};
    size_t const bu{b >> 31}, bd{b & mask31};
    size_t const m{ad*bu + au*bd};
    size_t const mu{m >> 30}, md{m & mask30};
    return au*bu*2 + mu+(md<<31) + ad*bd;
  }
  size_t _size;
  vector<size_t> _pow /* pow(base, i) % mod */, _hash /* hash of `str[0:i)` */;
  public:
  static auto hash(string_view const str) {
    size_t h{};
    for (char const c : str)
      h = (_mul_(h, Base::value()) + c) % Mod{};
    return h;
  }
  RollingHash() = default;
  explicit RollingHash(string_view const str)
  : _pow(str.size()+1, 1), _hash(str.size()+1, 0)
  {
    for (size_t i{0}; i < str.size(); ++i) {
      _pow[i+1] = _mul_(_pow[i], Base::value()) % Mod{};
      _hash[i+1] = (_mul_(_hash[i], Base::value()) + str[i]) % Mod{};
    }
  }
  auto substr(size_t const pos, size_t const len) const {
    assert(pos+len < _hash.size());
    constexpr size_t positivizer{Mod::value() * 4};
    return (_hash[pos+len] + positivizer - _mul_(_hash[pos], _pow[len])) % Mod{};
  }
  auto search(string_view const str) const {
    size_t const _h = hash(str);
    vector<size_t> idx;
    for (size_t i{0}; i+str.size() < _hash.size(); ++i)
      if (substr(i, str.size()) == _h)
        idx.push_back(i);
    return idx;
  }
  auto concat(size_t const h1, size_t const h2, size_t const h2len) const {
    return (_mul_(h1, _pow[h2len]) + h2) % Mod{};
  }
  void append(string_view const str) {
    _pow.reserve(_pow.size() + str.size());
    _hash.reserve(_hash.size() + str.size());
    for (size_t i{0}; i < str.size(); ++i) {
      _pow.push_back(
        _mul_(_pow.back(), Base::value()) % Mod{}
      );
      _hash.push_back(
        (_mul_(_hash.back(), Base::value()) + str[i]) % Mod{}
      );
    }
  }
};

/**
 * todo
 * ハッシュ化対象を文字列以外に拡張
 * concat の再考
 *   (Hash h1, Hash h2) -> Hash のようなインターフェースにしたい
 * static 変数のスレッドセーフ初期化 (コンパイラが自動生成) を除去 @ Base::value()
 *   -fno-threadsafe-statics オプション？
 */
