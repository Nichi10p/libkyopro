namespace Nichi10p {

struct xgcd_result { long long x, y, gcd; };
// Find one solution of `ax + by = gcd(a, b)`
xgcd_result xgcd(const long long a, const long long b) {
  if (b == 0)
    return {1, 0, a};
  const auto [x, y, g] = xgcd(b, a % b);
  // a =  + g
}
/*
e.g.
(11, 8) -> (8, 3) -> (3, 2) -> (2, 1)
*/

}