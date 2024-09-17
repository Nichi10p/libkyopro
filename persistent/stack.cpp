#include <iostream>
#include <utility>
#include <memory>
using namespace std;

template<typename T> class Stack {
  private:
  struct Node {
    shared_ptr<Node> const _prev;
    T const                _data;
    template<class... Args> Node(shared_ptr<Node> const &p, Args&&... args)
    : _prev(p), _data(std::forward<Args>(args)...) {}
  };
  shared_ptr<Node> const _this;
  Stack(shared_ptr<Node> const &s) : _this(s) {}
  Stack(shared_ptr<Node> &&s) : _this(std::move(s)) {}
  public:
  Stack() : _this(nullptr) {}
  bool     empty()          const { return !_this; }
  T const& top()            const { return _this->_data; }
  Stack    push(T const &x) const { return make_shared<Node>(_this, x); }
  Stack    push(T &&x)      const { return make_shared<Node>(_this, std::move(x)); }
  Stack    pop()            const { return _this->_prev; }
  template<typename... Args> Stack emplace(Args&&... args) const {
    return make_shared<Node>(_this, std::forward<Args>(args)...);
  }
  // dump (debug)
  friend ostream& operator<<(ostream& out, Stack const &s) {
    if (!s.empty()) {
      out << s.top() << '\n';
      out << s.pop();
    }
    return out;
  }
};
