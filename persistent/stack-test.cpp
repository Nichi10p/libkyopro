#include <cassert>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <stack>

#include "stack.cpp"


char const *ftestdata = "input.txt";
char const *fresult = "output.txt";
char const *fanswer = "answer.txt";

void print_std_stack(ostream &out, stack<int> sta /* copy */) {
  while (!sta.empty()) {
    out << sta.top() << '\n';
    sta.pop();
  }
}

/*
 * size-of-query
 * version query-type value
 * ...
 */
void make_test() {
  ofstream dat(ftestdata);
  ofstream ans(fanswer);
  random_device dev;
  mt19937_64 ran(dev());

  int Q{100};
  dat << Q << '\n';
  vector Vec{stack<int>{}};
  while (Q--) {
    auto const ver{ran() % Vec.size()};
    auto const t{ran() % 4};          // 0, 1 ~ 3
    auto const val{ran() % 90 + 10};  // 10 ~ 99
    stack<int> sta{Vec.at(ver)};
    if (sta.empty() || t) {
      dat << ver << ' ' << 1 << ' ' << val << '\n';
      sta.push((int)val);
    }
    else {
      dat << ver << ' ' << 0 << ' ' << val << '\n';
      sta.pop();
    }
    Vec.emplace_back(std::move(sta));
  }

  for (stack<int> const &st : Vec) {
    ans << "===\n";
    print_std_stack(ans, st);
    ans << "===\n";
  }
}

void run_test() {
  ifstream dat(ftestdata);
  ofstream res(fresult);

  int Q;
  dat >> Q;
  vector Vec{Stack<int>{}};
  while (Q--) {
    int ver, t, val;
    dat >> ver >> t >> val;
    if (t) {
      Vec.emplace_back(Vec.at(ver).push(val));
    }
    else {
      assert(not Vec.at(ver).empty());
      Vec.emplace_back(Vec.at(ver).pop());
    }
  }

  for (size_t i{}; i < Vec.size(); ++i) {
    res << "===\n";
    res << Vec.at(i);
    res << "===\n";
  }
}

bool check_result() {
  ifstream ans(fanswer);
  ifstream res(fresult);

  string sa, sr;
  while (!ans.eof() && !res.eof()) {
    getline(ans, sa);
    getline(res, sr);
    if (sa != sr)
      return false;
  }

  if (!ans.eof() || !res.eof())
    return false;
  return true;
}

int main() {
  make_test();
  run_test();
  assert(check_result());
}
