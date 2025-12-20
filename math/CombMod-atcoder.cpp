#include <cmath>
#include <vector>
#include "atcoder/modint"
typedef atcoder::modint998244353 Int;

/**
 * @brief comb(n, r) % MOD
 * @note construct O(N)
 * @note query O(1)
 * @warning MOD must be prime
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
struct CombMod {
  std::vector<Int> _fact, _fact_inv, _inv;
  explicit CombMod(int const _n)
  : _fact(_n+1), _fact_inv(_n+1), _inv(_n+1)
  {
    _fact[0]     = _fact[1]     = Int::raw(1);
    _fact_inv[0] = _fact_inv[1] = Int::raw(1);
    _inv[1] = Int::raw(1);
    for (int _i=2; _i <= _n; ++_i) {
      auto const [_q, _r] = std::div(Int::mod(), _i);
      _inv[_i] = -Int(_q) * _inv[_r];
      _fact[_i] = _fact[_i-1] * Int::raw(_i);
      _fact_inv[_i] = _fact_inv[_i-1] * _inv[_i];
    }
  }
  Int comb(int const _n, int const _r) const {
    if (_r < 0 || _n < _r)
      return 0;
    else
      return _fact.at(_n) * _fact_inv[_n-_r] * _fact_inv[_r];
  }
  Int multicomb(int const _n, int const _r) const {
    return comb(_n+_r-1, _r);
  }
};
