#include <cassert>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
using namespace std;

constexpr int MOD_STATIC = 31;
chrono::system_clock::time_point s, t;

ostream& clog_time(char const *name) {
  auto const dur = chrono::duration_cast<chrono::microseconds>(t - s);
  auto const [x, y] = div(dur.count(), (int64_t)1000);
  return clog << setw(4) << setfill(' ') << x << '.' << setw(3) << setfill('0') << y << "ms @" << name;
}

#if 0
#include "CombMod-prime.cpp"
void solve_prime_1(
  int const MOD, int const N, [[maybe_unused]] int const maxR, int const Q,
  vector<int> const &R, vector<int> &ans
)
{
  assert(MOD == MOD_STATIC);
  s = chrono::system_clock::now();
  type1::CombMod<MOD_STATIC> math(N);
  t = chrono::system_clock::now();
  clog_time("construct") << ", ";
  s = chrono::system_clock::now();
  for (int i=0; i < Q; ++i)
    ans[i] = math.comb(N, R[i]);
  t = chrono::system_clock::now();
  clog_time("solve") << '\n';
}
void solve_prime_2(
  int const MOD, int const N, int const maxR, int const Q,
  vector<int> const &R, vector<int> &ans
)
{
  assert(MOD == MOD_STATIC);
  s = chrono::system_clock::now();
  type2::CombMod<MOD_STATIC> math(N, maxR);
  t = chrono::system_clock::now();
  clog_time("construct") << ", ";
  s = chrono::system_clock::now();
  for (int i=0; i < Q; ++i)
    ans[i] = math.comb(R[i]);
  t = chrono::system_clock::now();
  clog_time("solve") << '\n';
}
#endif

#if 0
#include "CombMod-pascal.cpp"
void solve_pascal(
  int const MOD, int const N, [[maybe_unused]] int const maxR, int const Q,
  vector<int> const &R, vector<int> &ans
)
{
  assert(MOD == MOD_STATIC);
  s = chrono::system_clock::now();
  impl_b::CombMod<MOD_STATIC> math(N);
  t = chrono::system_clock::now();
  clog_time("construct") << ", ";
  s = chrono::system_clock::now();
  for (int i=0; i < Q; ++i)
    ans[i] = math.comb(N, R[i]);
  t = chrono::system_clock::now();
  clog_time("solve") << '\n';
}
#endif

#if 1
#include "CombMod-lucas.cpp"
void solve_lucas(
  int const MOD, int const N, [[maybe_unused]] int const maxR, int const Q,
  vector<int> const &R, vector<int> &ans
)
{
  assert(MOD == MOD_STATIC);
  s = chrono::system_clock::now();
  CombMod<MOD_STATIC> math;
  t = chrono::system_clock::now();
  clog_time("construct") << ", ";
  s = chrono::system_clock::now();
  for (int i=0; i < Q; ++i)
    ans[i] = math.comb(N, R[i]);
  t = chrono::system_clock::now();
  clog_time("solve") << '\n';
}
#endif

int main() {
  int MOD, N, maxR, Q;
  cin >> MOD >> N >> maxR >> Q;
  vector<int> R(Q), ans(Q);
  for (int i=0; i < Q; ++i)
    cin >> R[i];

  // solve_prime_1(MOD, N, maxR, Q, R, ans);
  // solve_prime_2(MOD, N, maxR, Q, R, ans);
  // solve_pascal(MOD, N, maxR, Q, R, ans);
  solve_lucas(MOD, N, maxR, Q, R, ans);

  for (int const a : ans)
    cout << a << '\n';
}
