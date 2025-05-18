#include <cassert>
#include <vector>
using namespace std;
typedef long long ll;

/**
 * @brief comb(n, r) % MOD
 * @note construct O(MOD ^ 2)
 * @note query O(log n)
 * @warning MOD must be prime
 * @see https://algo-logic.info/binomial-coefficient-lucas/
 */
template<int MOD=3> class CombMod {
  vector<vector<ll>> _dp;
  public:
  explicit CombMod(int const N)
  : _dp(MOD, vector<ll>(MOD))
  {
    _dp[0][0] = 1;
    for (int n=1; n < MOD; ++n) {
      _dp[n][0] = 1;
      for (int r=1; r <= n; ++r)
        _dp[n][r] = (_dp[n-1][r-1] + _dp[n-1][r]) % MOD;
    }
  }
  ll comb(ll const n, ll const r) {
    if (r < 0 || n < r)
      return 0;
    ll _ret = 1;
    while (n) {
      _ret *= _dp[n % MOD][r % MOD];
      n /= MOD;
      r /= MOD;
      _ret %= MOD;
    }
    return _ret;
  }
  ll multicomb(ll const n, ll const r) {
    return comb(n+r-1, r);
  }
};

/**
 * @brief comb(n, r) % MOD
 * @note construct O(1)
 * @note query O(1)
 * @warning MOD must be prime
 * @see https://algo-logic.info/is-nck-odd-even/
 */
template<> class CombMod<2> {
  public:
  CombMod() {
  }
  explicit CombMod(int) {
  }
  int comb(ll const n, ll const r) {
    return (n & r) == r;
  }
  int multicomb(ll const n, ll const r) {
    return comb(n+r-1, r);
  }
};
