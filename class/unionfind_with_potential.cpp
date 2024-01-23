#include <type_traits>
#include <utility>
#include <tuple>
#include <vector>
#include <functional>

namespace nichi10p {
  // refer
  // github.com/atcoder/ac-library/blob/master/atcoder/dsu.hpp
  // github.com/noshi91/NoshiMochiLibrary/blob/master/Tree/UnionFindTree/PotentializedUnionFind.noshi.cpp
  // qiita.com/drken/items/cce6fc5c579051e64fab
  template<typename potential_t>  // potential_t must have `plus`, `minus`, `0` instance (default constructed)
  class unionfind_with_potential {
    private:
      std::vector<int>                  par;  // par[i] := (i is not root) ? (parent node) : -(component size)
      std::vector<potential_t>          ptt;  // potential
      std::pair<int, potential_t> const _leader(int);  // {leader, potential}
    public:
      /* constructor */ explicit unionfind_with_potential(int const &);
      int const         leader(int);
      potential_t const potential(int);
      void              merge(int, int, potential_t);
      bool const        same(int const &, int const &);
      potential_t const diff(int const &, int const &);
      int const         size(int const &);
      // std::vector<std::vector<int>> const groups();
  };
}

namespace nichi10p {
  // potential_t must have `plus`, `minus`, `0` instance (default constructed)
  template<typename potential_t>
  unionfind_with_potential<potential_t>::unionfind_with_potential(int const &N)
  : par(N, -1), ptt(N)
  {
    static_assert(
      std::is_invocable<std::plus<>, potential_t, potential_t>::value,
      "`potential_t + potential_t` failed. (binary operator+)"
    );
    static_assert(
      std::is_invocable<std::minus<>, potential_t, potential_t>::value,
      "`potential_t - potential_t` failed. (binary operator-)"
    );
    static_assert(
      std::is_default_constructible<potential_t>::value,
      "`potential_t{}` failed. (default construct)"
    );
  }

  template<typename potential_t>
  std::pair<int, potential_t> const unionfind_with_potential<potential_t>::_leader(int x) {
    potential_t p{};
    // path halving
    while (par[x] >= 0 && par[par[x]] >= 0) {
      ptt[x] = std::plus{}(ptt[par[x]], ptt[x]);
      p = std::plus{}(ptt[x], p);
      x = par[x] = par[par[x]];
    }
    if (par[x] < 0)
      return {x, p};
    else
      return {par[x], std::plus{}(ptt[x], p)};
  }

  template<typename potential_t>
  int const unionfind_with_potential<potential_t>::leader(int x) {
    return _leader(x).first;
  }

  template<typename potential_t>
  potential_t const unionfind_with_potential<potential_t>::potential(int x) {
    return _leader(x).second;
  }

  template<typename potential_t>
  void unionfind_with_potential<potential_t>::merge(int x, int y, potential_t d) {
    auto const [lx, px] = _leader(x);
    auto const [ly, py] = _leader(y);
    d = std::minus{}(std::plus{}(px, d), py);
    std::tie(x, y) = std::tuple(lx, ly);
    if (-par[x] < -par[y]) {
      std::swap(x, y);
      d = std::minus{}(potential_t{}, d);  // 0 - d
    }
    par[x] += par[y];
    par[y] = x;
    ptt[y] = d;
  }

  template<typename potential_t>
  bool const unionfind_with_potential<potential_t>::same(int const &x, int const &y) {
    return leader(x) == leader(y);
  }

  template<typename potential_t>
  potential_t const unionfind_with_potential<potential_t>::diff(int const &x, int const &y) {
    return std::minus{}(potential(y), potential(x));
  }

  template<typename potential_t>
  int const unionfind_with_potential<potential_t>::size(int const &x) {
    return -par[leader(x)];
  }
}
