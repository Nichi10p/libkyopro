#include <concepts>
#include <functional>
#include <utility>
#include <vector>
#include <iterator>
#include <ranges>
#include <algorithm>

// C++20 or later
// merge-insertion sort (Ford-Johnson algorithm)
// https://en.wikipedia.org/wiki/Merge-insertion_sort
template<std::ranges::random_access_range R, class Comp = std::ranges::less, class Proj = std::identity>
  requires
  std::sortable<std::ranges::iterator_t<R>, Comp, Proj> &&
  std::copy_constructible<std::ranges::range_value_t<R>> &&
  std::equality_comparable<std::ranges::range_value_t<R>>
void fjasort(R &&r, Comp cp = {}, Proj pj = {}) {
  using T = std::ranges::range_value_t<R>;
  size_t const n = std::ranges::size(r);
  if (n < 2)
    return;
  // pairing
  // if n is odd, the last element of r is unpaired
  std::vector<std::pair<T,T>> P(n/2);
  for (auto itr = std::ranges::begin(r); auto &[fir, sec] : P) {
    fir = *itr++;
    sec = *itr++;
  }
  // compare
  std::vector<T> S(n/2);
  for (auto itr = S.begin(); auto &[fir, sec] : P) {
    if (std::invoke(cp, std::invoke(pj, fir), std::invoke(pj, sec)))
      std::ranges::swap(fir, sec);
    *itr++ = fir;  // fir > sec
  }
  // recursively sort
  fjasort(S, cp, pj);
  for (auto itr = P.begin(); auto const &x : S) {
    auto jtr = std::ranges::find(P, x, &std::pair<T,T>::first);
    if (itr != jtr)
      std::ranges::iter_swap(itr, jtr);
    ++itr;
  }
  if (n%2 == 1)
    P.emplace_back(*std::ranges::begin(r) /* fake */, *std::ranges::rbegin(r));
  // insert
  // z (patition size) = 2, 2, 6, 10, 22, 42, ... (oeis.org/A078008)
  auto Insert = [&](size_t z) {
    z = std::min(z, P.size());
    std::reverse(P.begin(), P.begin() + z);
    for (size_t i{0}; i < z; ++i) {
      auto end = P[i].second == *std::ranges::rbegin(r) ? S.end() : std::ranges::find(S, P[i].first);
      auto pos = std::ranges::lower_bound(S.begin(), end, P[i].second, cp, pj);
      S.insert(pos, P[i].second);
    }
    P.erase(P.begin(), P.begin() + z);
  };
  S.insert(S.begin(), P[0].second);  // P[0].second < P[0].first
  P.erase(P.begin());
  for (size_t const n : {2, 2, 6})
    Insert(n);
  for (size_t n3=2,n2=2,n1=6,n; not P.empty(); n3=n2,n2=n1,n1=n) {
    n = 3*n2 + 2*n3;
    Insert(n);
  }
  // result
  std::ranges::copy(S, std::ranges::begin(r));
}