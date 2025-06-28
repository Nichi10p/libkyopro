#include <cassert>
#include <vector>
using namespace std;

namespace type1 {
/**
 * @brief comb(n, r) % MOD
 * @note construct O(N)
 * @note query O(1)
 * @warning MOD must be prime
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<int MOD=998244353> class CombMod {
  vector<int> _fact, _fact_inv;
  public:
  explicit CombMod(int const N)
  : _fact(N+1), _fact_inv(N+1)
  {
    vector<int> _inv(N+1);
    _inv[1] = 1;
    _fact[0] = 1;
    _fact[1] = 1;
    _fact_inv[0] = 1;
    _fact_inv[1] = 1;
    for (int i=2; i <= N; ++i) {
      _inv[i] = (long long)(MOD - MOD/i) * _inv[MOD%i] % MOD;
      _fact[i] = (long long)_fact[i-1] * i % MOD;
      _fact_inv[i] = (long long)_fact_inv[i-1] * _inv[i] % MOD;
    }
  }
  int comb(int const n, int const r) const {
    assert(n < (int)_fact.size());
    if (r < 0 || n < r)
      return 0;
    int _ret = (long long)_fact[n] * _fact_inv[n-r] % MOD * _fact_inv[r] % MOD;
    return _ret;
  }
  int multicomb(int const n, int const r) const {
    return comb(n+r-1, r);
  }
};
} // namespace type1

namespace type2 {
/**
 * @brief comb(n, r) % MOD, fixed n
 * @note construct O(R)
 * @note query O(1)
 * @warning MOD must be prime
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<int MOD=998244353> class CombMod {
  vector<int> _inv, _nC;
  public:
  CombMod(int const N, int const R)
  : _inv(R+1), _nC(R+1)
  {
    _inv[1] = 1;
    _nC[0] = 1;
    _nC[1] = N % MOD;
    for (int i=2; i <= R; ++i) {
      _inv[i] = (long long)(MOD - MOD/i) * _inv[MOD%i] % MOD;
      _nC[i] = (long long)_nC[i-1] * (N-i+1) % MOD * _inv[i] % MOD;
    }
  }
  int comb(int const r) const {
    assert(r < (int)_nC.size());
    return _nC[r];
  }
  // query O(r)
  int comb(int const n, int r) const {
    if (r < 0 || n < r)
      return 0;
    if (n-r < r)
      r = n-r;
    assert(r < (int)_inv.size());
    int _ret = 1;
    for (int i=1; i <= r; ++i)
      _ret = (long long)_ret * (n-i+1) % MOD * _inv[i] % MOD;
    return _ret;
  }
  // query O(r)
  int multicomb(int const n, int const r) const {
    return comb(n+r-1, r);
  }
};
} // namespace type2
