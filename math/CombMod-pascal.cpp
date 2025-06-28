#include <cassert>
#include <vector>
using namespace std;
typedef long long ll;

namespace impl_a {
/**
 * @brief comb(n, r) % MOD
 * @note construct O(N^2)
 * @note query O(1)
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<ll MOD=0> class CombMod {
  vector<vector<ll>> _dp;
  public:
  explicit CombMod(int const N)
  : _dp(N+1, vector<ll>(N+1))
  {
    _dp[0][0] = 1;
    for (int n=1; n <= N; ++n) {
      _dp[n][0] = _dp[n-1][0];
      for (int r=1; r < n; ++r) {
        _dp[n][r] = _dp[n-1][r-1] + _dp[n-1][r];
        if constexpr (MOD > 0)
          _dp[n][r] %= MOD;
      }
      _dp[n][n] = _dp[n-1][n-1];
    }
  }
  ll comb(int const n, int const r) const {
    if (r < 0 || n < r)
      return 0;
    assert(n < (int)_dp.size());
    return _dp[n][r];
  }
  ll multicomb(int const n, int const r) const {
    return comb(n+r-1, r);
  }
};
} // namespace impl_a

namespace impl_b {
/**
 * @brief comb(n, r) % MOD
 * @note construct O(N^2)
 * @note query O(1)
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<ll MOD=0> class CombMod {
  vector<ll> _dp;
  ll& _at(int const i, int const j) {
    // [0,0]
    // [1,0], [1,1]
    // [2,0], [2,1], [2,2]
    // [3,0], [3,1], [3,2], [3,3]
    // ...
    // (1 + 2 + ... + i) + j
    return _dp[(i+1)*i/2 + j];
  }
  public:
  explicit CombMod(int const N)
  : _dp((N+1)*N/2 + N + 1)
  {
    _at(0, 0) = 1;
    for (int n=1; n <= N; ++n) {
      _at(n, 0) = _at(n-1, 0);
      for (int r=1; r < n; ++r) {
        _at(n, r) = _at(n-1, r-1) + _at(n-1, r);
        if constexpr (MOD > 0)
          _at(n, r) %= MOD;
      }
      _at(n, n) = _at(n-1, n-1);
    }
  }
  ll comb(int const n, int const r) const {
    if (r < 0 || n < r)
      return 0;
    assert((ll)(n+1)*n/2 + r < (int)_dp.size());
    return _at(n, r);
  }
  ll multicomb(int const n, int const r) const {
    return comb(n+r-1, r);
  }
};
} // namespace impl_b
