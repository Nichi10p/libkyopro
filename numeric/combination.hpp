#include <cassert>
#include <vector>

namespace Nichi10p {

class CombMod {
 public:
  virtual long long nPr(long long, long long) = 0;
  virtual long long nCr(long long, long long) = 0;
  virtual long long nHr(long long, long long) = 0;
};

namespace CombModImpls {
// construct O(N²), query O(1)
class DP : CombMod {
 public:
  DP(int N, int mod) : _dp(N+1, std::vector<long long>(N+1)) {
    assert(N > 0);
    assert(mod > 0);
    _dp[0][0] = 1 % mod;
    for (int n = 1; n <= N; ++n) {
      _dp[n][0] = 1 % mod;
      for (int r = 1; r <= n; ++r)
        _dp[n][r] = (_dp[n-1][r-1] + _dp[n-1][r]) % mod;
    }
  }
  DP(int N) : _dp(N+1, std::vector<long long>(N+1)) {
    assert(N > 0);
    _dp[0][0] = 1;
    for (int n = 1; n <= N; ++n) {
      _dp[n][0] = 1;
      for (int r = 1; r <= n; ++r)
        _dp[n][r] = _dp[n-1][r-1] + _dp[n-1][r];
    }
  }
  long long nCr(long long n, long long r) override {
    assert(0 <= r && r <= n);
    return _dp[n][r];
  }
  long long nHr(long long n, long long r) override {
    ;
    return _dp[n+r-1][r];
  }
 private:
  std::vector<std::vector<long long>> _dp;
};

} // namespace CombModImpl
} // namespace Nichi10p