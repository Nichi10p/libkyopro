#include <cassert>
#include <vector>
using namespace std;

/**
 * @brief calculate \f$ comb(n,r) \% mod \f$ with [atcoder::modint](https://atcoder.github.io/ac-library/production/document_ja/modint.html)-like class
 * construct \f$ O(N^2) \f$
 * query \f$ O(1) \f$
 * @ref https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<typename modint> class CombModDP {
  public:
  explicit CombModDP(int const N) : _dp(N+1, vector<modint>(N+1)) {
    assert(N > 0);
    _dp[0][0] = modint(1);
    for (int n=1; n <= N; ++n) {
      _dp[n][0] = modint(1);
      for (int r=1; r <= n; ++r)
        _dp[n][r] = _dp[n-1][r-1] + _dp[n-1][r];
    }
  }
  modint comb(int n, int r) {
    assert(0 <= n && n < (int)_dp.size());
    if (n < r)
      return modint(0);
    else
      return _dp[n][r];
  }
  modint multicomb(int n, int r) {
    return comb(n+r-1, r);
  }
  private:
  vector<vector<modint>> _dp;
};
