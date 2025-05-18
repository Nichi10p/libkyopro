#include <cassert>
#include <vector>
using namespace std;
typedef long long ll;

/**
 * @brief comb(n, r) % MOD
 * @note construct O(N^2)
 * @note query O(1)
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<ll MOD=(1LL << 62)> class CombMod {
  vector<vector<ll>> _dp;
  public:
  explicit CombMod(int const N)
  : _dp((assert(N > 0), N+1), vector<ll>(N+1))
  {
    _dp[0][0] = 1;
    for (int n=1; n <= N; ++n) {
      _dp[n][0] = 1;
      for (int r=1; r <= n; ++r)
        _dp[n][r] = (_dp[n-1][r-1] + _dp[n-1][r]) % MOD;
    }
  }
  ll comb(int const n, int const r) {
    assert(n < (int)_dp.size());
    if (r < 0 || n < r)
      return 0;
    else
      return _dp[n][r];
  }
  ll multicomb(int const n, int const r) {
    return comb(n+r-1, r);
  }
};
