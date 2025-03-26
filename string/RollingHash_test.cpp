#include <iostream>
#include "RollingHash.cpp"  // using namespace std;

auto random_string(int n) {
  static mt19937_64 mt;
  static uniform_int_distribution<int> ran(0, 25);
  string ret(n, 'a');
  for (int i=0; i < n; ++i)
    ret[i] += ran(mt);
  return ret;
}

int main() {
  int t=100000;
  int ng=0;
  while (t--) {
    string _s = random_string(20);
    string _t = random_string(20);
    string S = _s + _t;
    RollingHash RH(_s);
    string method;
    if (t%3 == 0) {
      for (char const c : _t)
        RH.push_back(c);
      method = "push_back";
    }
    if (t%3 == 1) {
      RH += _t;
      method = "+= (string)";
    }
    if (t%3 == 2) {
      RH += RollingHash(_t);
      method = "+= (RollingHash)";
    }
    string x = random_string(2);
    if (S.starts_with(x) != RH.starts_with(x)) {
      ++ng;
      cout << "failed @starts_with (build by " << method << ")\n";
      cout << "S=" << S << ", x=" << x << ", result=" << RH.starts_with(x) << endl;
    }
    if (S.ends_with(x) != RH.ends_with(x)) {
      ++ng;
      cout << "failed @ends_with (build by " << method << ")\n";
      cout << "S=" << S << ", x=" << x << ", result=" << RH.ends_with(x) << endl;
    }
    if (S.find(x) != RH.find(x)) {
      ++ng;
      cout << "failed @find (build by " << method << ")\n";
      cout << "S=" << S << ", x=" << x << ", result=" << RH.find(x) << endl;
    }
    if (S.rfind(x) != RH.rfind(x)) {
      ++ng;
      cout << "failed @rfind (build by " << method << ")\n";
      cout << "S=" << S << ", x=" << x << ", result=" << RH.find(x) << endl;
    }
    if (ng >= 10)
      break;
  }
  if (ng == 0)
    cout << "OK" << endl;
}