#include <cassert>
#include <vector>
using namespace std;

/**
 * @brief calculate \f$ comb(n,r) \% mod \f$ with [atcoder::modint](https://atcoder.github.io/ac-library/production/document_ja/modint.html)-like class
 * construct \f$ O(N) \f$
 * query \f$ O(1) \f$
 * mod must be prime
 * @ref https://drken1215.hatenablog.com/entry/2018/06/08/210000
 */
template<typename modint> class CombMod {
  public:
  explicit CombMod(int const N) : _fact(N+1), _inv(N+1), _fact_inv(N+1) {
    assert(N > 0);
    int const MOD = modint::mod();
    _fact[0] = modint(1);
    _fact[1] = modint(1);
    // _inv[0]: undefined
    _inv[1] = modint(1);
    _fact_inv[0] = modint(1);
    _fact_inv[1] = modint(1);
    for (int i=2; i <= N; ++i) {
      _fact[i] = _fact[i-1] * i;
      _inv[i] = modint(MOD - MOD / i) * _inv[MOD % i];
      _fact_inv[i] = _fact_inv[i-1] * _inv[i];
    }
  }
  modint fact(int const n) {
    assert(0 <= n && n < (int)_fact.size());
    return _fact[n];
  }
  modint perm(int const n, int const r) {
    assert(0 <= r);
    if (n < r)
      return modint(0);
    else
      return fact(n) * _fact_inv[n-r];
  }
  modint comb(int const n, int const r) {
    if (n < r)
      return modint(0);
    else
      return perm(n, r) * _fact_inv[r];
  }
  modint multicomb(int const n, int const r) {
    return comb(n+r-1, r);
  }
  private:
  vector<modint> _fact, _inv, _fact_inv;
};
