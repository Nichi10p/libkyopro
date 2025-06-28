typedef long long ll;

/**
 * @brief comb(n, r) % MOD
 * @note construct O(MOD ^ 2)
 * @note query O(log n)
 * @warning MOD must be prime
 * @see https://algo-logic.info/binomial-coefficient-lucas/
 */
template<int MOD> class CombMod {
  int _dp[MOD][MOD];
  public:
  CombMod()
  {
    _dp[0][0] = 1;
    for (int j=1; j < MOD; ++j)
      _dp[0][j] = 0;
    for (int n=1; n < MOD; ++n) {
      _dp[n][0] = _dp[n-1][0];
      for (int r=1; r < n; ++r)
        _dp[n][r] = (_dp[n-1][r-1] + _dp[n-1][r]) % MOD;
      _dp[n][n] = _dp[n-1][n-1];
      for (int r=n+1; r < MOD; ++r)
        _dp[n][r] = 0;
    }
  }
  int comb(ll n, ll r) const {
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
  int multicomb(ll const n, ll const r) const {
    return comb(n+r-1, r);
  }
};

/**
 * @brief comb(n, r) % MOD
 * @note construct O(1)
 * @note query O(1)
 * @see https://algo-logic.info/is-nck-odd-even/
 */
template<> class CombMod<2> {
  public:
  int comb(ll const n, ll const r) const {
    if (r < 0 || n < r)
      return 0;
    return (n & r) == r;
  }
  int multicomb(ll const n, ll const r) const {
    return comb(n+r-1, r);
  }
};
