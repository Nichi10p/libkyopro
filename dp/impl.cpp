#include <vector>
#include <functional>
using namespace std;

template<class T>
class RerootingTreeDP {
private:
    vector<vector<int>> _graph;  // 隣接リスト
    vector<vector<T>> _dp;       // 頂点 v と _graph[v][i] 以下からなる木のDP結果
    vector<T> _answer;           // 頂点 v を根とした木のDP結果

    const function<T(const T&, const T&)> _merge;  // 2つのDP結果をmergeする
    const function<T(const T&)> _add_root;         // 頂点 v をｋ
    const T _e;                                    // 単位元  _merge(x, _e) == x

    // v を根とした部分木のDP
    T dp(const int v, const int parent) {
        T result = _e;
        int sz = static_cast<int>(_graph[v].size());
        _dp[v].resize(sz);
        // DP
        for (int i = 0; i < sz; ++i)
            if (_graph[v][i] != parent) {
                _dp[v][i] = _add_root(dp(_graph[v][i], v));
                result = _merge(result, _dp[v][i]);
            }
        return result;
    }
    // v を根とした木DPを parent を根とした部分木のDP結果 (part) を利用して求める
    void rerooting(const int v, const int parent, const T &part) {
        int sz = static_cast<int>(_graph[v].size());
        // part を _dp に反映
        for (int i = 0; i < sz; ++i)
            if (_graph[v][i] == parent)
                _dp[v][i] = _add_root(part);
        // 必要な値の収集の高速化 (累積和)
        auto left  = vector(sz, _e);  // [0, i)
        auto right = vector(sz, _e);  // (i, sz - 1]
        for (int i = 1; i < sz; ++i) {
            left[i] = _merge(left[i - 1], _dp[v][i - 1]);
            const int j = sz - 1 - i;
            right[j] = _merge(right[j + 1], _dp[v][j + 1]);
        }
        // DP
        for (int i = 0; i < sz; ++i)
            if (_graph[v][i] != parent)
                rerooting(_graph[v][i], v, _merge(left[i], right[i]));
        _answer[v] = _merge(_dp[v][0], right[0]);
    }
public:
    template<class F, class G> RerootingTreeDP(int n, F merge, G add_root, T e)
    : _graph(n), _dp(n), _answer(n), _merge(merge), _add_root(add_root), _e(e) {
    }
    void add_edge(const int u, const int v) {
        // 無向辺を想定している
        _graph[u].push_back(v);
        _graph[v].push_back(u);
    }
    T first(const int root) {
        return _answer[root] = dp(root, -1);
    }
    void second(const int root) {
        rerooting(root, -1, _e);
    }
    T operator[](int v) const {
        return _answer[v];
    }
};
