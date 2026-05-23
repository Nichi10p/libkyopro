#include <cassert>
#include <cmath>
#include <vector>

/**
 * @brief 二項係数 % MOD を計算する
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000 (実装について)
 * @see https://qiita.com/drken/items/f2ea4b58b0d21621bd51 (写像12相)
 */
template<int MOD=998244353> struct CombMod {
  std::vector<int> _fact, _fact_inv, _inv;
  explicit CombMod(int const N)
  : _fact(N+1), _fact_inv(N+1), _inv(N+1)
  {
    _fact[0]     = _fact[1]     = 1;
    _fact_inv[0] = _fact_inv[1] = 1;
    _inv[1] = 1;
    for (int i=2; i <= N; ++i) {
      auto const [q, r] = std::div(MOD, i);
      _inv[i] = (long long)(MOD - q) * _inv[r] % MOD;
      _fact[i] = (long long)_fact[i-1] * i % MOD;
      _fact_inv[i] = (long long)_fact_inv[i-1] * _inv[i] % MOD;
    }
  }
  long long comb(int const n, int const r) const {
    if (r < 0 || n < r)
      return 0;
    long long ret = _fact.at(n);
    ret *= _fact_inv[n-r]; ret %= MOD;
    ret *= _fact_inv[r];   ret %= MOD;
    return ret;
  }
};

/**
 * @brief 二項係数 % MOD を計算する (N が大きく R が小さいとき)
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000 (実装について)
 * @see https://qiita.com/drken/items/f2ea4b58b0d21621bd51 (写像12相)
 */
template<int MOD=998244353> struct CombMod {
  std::vector<int> _inv;
  explicit CombMod(int const R)
  : _inv(R+1)
  {
    _inv[1] = 1;
    for (int i=2; i <= R; ++i) {
      auto const [q, r] = std::div(MOD, i);
      _inv[i] = (long long)(MOD - q) * _inv[r] % MOD;
    }
  }
  long long comb(long long const n, int r) const {
    if (r < 0 || n < r)
      return 0;
    assert(r < (int)_inv.size());
    long long ret = 1;
    for (int i=1; i <= r; ++i) {
      ret *= n-i+1;   ret %= MOD;
      ret *= _inv[i]; ret %= MOD;
    }
    return ret;
  }
};
