#include <cmath>
#include <vector>
#include "atcoder/modint"
typedef atcoder::modint998244353 Int;

/**
 * @brief 二項係数 % MOD を計算する
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000 (実装について)
 * @see https://qiita.com/drken/items/f2ea4b58b0d21621bd51 (写像12相)
 */
struct CombMod {
  std::vector<Int> _fact, _fact_inv, _inv;
  explicit CombMod(int const N)
  : _fact(N+1), _fact_inv(N+1), _inv(N+1)
  {
    _fact[0]     = _fact[1]     = Int::raw(1);
    _fact_inv[0] = _fact_inv[1] = Int::raw(1);
    _inv[1] = Int::raw(1);
    for (int i=2; i <= N; ++i) {
      auto const [q, r] = std::div(Int::mod(), i);
      _inv[i] = -Int(q) * _inv[r];
      _fact[i] = _fact[i-1] * Int::raw(i);
      _fact_inv[i] = _fact_inv[i-1] * _inv[i];
    }
  }
  Int comb(int const n, int const r) const {
    if (r < 0 || n < r)
      return 0;
    else
      return _fact.at(n) * _fact_inv[n-r] * _fact_inv[r];
  }
};

/**
 * @brief 二項係数 % MOD を計算する (N が大きく R が小さいとき)
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000 (実装について)
 * @see https://qiita.com/drken/items/f2ea4b58b0d21621bd51 (写像12相)
 */
struct CombMod {
  std::vector<Int> _inv;
  explicit CombMod(int const R)
  : _inv(R+1)
  {
    _inv[1] = Int::raw(1);
    for (int i=2; i <= R; ++i) {
      auto const [q, r] = std::div(Int::mod(), i);
      _inv[i] = -Int(q) * _inv[r];
    }
  }
  Int comb(long long const n, int r) const {
    if (r < 0 || n < r)
      return 0;
    assert(r < (int)_inv.size());
    Int ret(1);
    for (int i=1; i <= r; ++i) {
      ret *= n-i+1;
      ret *= _inv[i];
    }
    return ret;
  }
};
