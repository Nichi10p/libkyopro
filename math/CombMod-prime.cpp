#include <cassert>
#include <vector>
using namespace std;
typedef long long ll;

/**
 * @brief comb(n, r) % MOD
 * @note construct O(N)
 * @note query O(1)
 * @warning MOD must be prime
 * @see https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<int MOD=998244353> class CombMod {
  vector<ll> _fact, _inv, _fact_inv;
  public:
  explicit CombMod(int const N)
  : _fact((assert(N > 0), N+1)), _inv(N+1), _fact_inv(N+1)
  {
    _fact[0] = 1;
    _fact[1] = 1;
    // _inv[0]: undefined
    _inv[1] = 1;
    _fact_inv[0] = 1;
    _fact_inv[1] = 1;
    for (int i=2; i <= N; ++i) {
      _fact[i] = _fact[i-1] * i % MOD;
      _inv[i] = (MOD - MOD/i) * _inv[MOD%i] % MOD;
      _fact_inv[i] = _fact_inv[i-1] * _inv[i] % MOD;
    }
  }
  ll comb(int const n, int const r) {
    assert(n < (int)_fact.size());
    if (r < 0 || n < r)
      return 0;
    ll _ret = _fact[n];
    _ret *= _fact_inv[n-r];
    _ret %= MOD;
    _ret *= _fact_inv[r];
    _ret %= MOD;
    return _ret;
  }
  ll multicomb(int const n, int const r) {
    return comb(n+r-1, r);
  }
};

/**
 * @brief comb(n, r) % MOD
 * @note construct O(R)
 * @note query O(r)
 * @warning MOD must be prime
 */
template<int MOD=998244353> class CombMod {
  vector<ll> _inv;
  public:
  explicit CombMod(int const R)
  : _inv((assert(R > 0), R+1))
  {
    // _inv[0]: undefined
    _inv[1] = 1;
    for (int i=2; i <= R; ++i)
      _inv[i] = (MOD - MOD/i) * _inv[MOD%i] % MOD;
  }
  ll comb(int const n, int const r) {
    assert(r < (int)_fact.size());
    if (r < 0 || n < r)
      return 0;
    ll _ret = 1;
    for (int i=0; i < r; ++i) {
      _ret *= (n-i) * _inv[i+1] % MOD;
      _ret %= MOD;
    }
    return _ret;
  }
  ll multicomb(int const n, int const r) {
    return comb(n+r-1, r);
  }
};
