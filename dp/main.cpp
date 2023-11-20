// https://algo-logic.info/tree-dp/

#include <vector>
#include <algorithm>

// ABC220 F
// https://atcoder.jp/contests/abc220/tasks/abc220_f
class Node {
public:
    long long dist_sum;
    int subtree_size;

    // 単位元
    static Node e() {
        return {0, 0};
    }
    // 2つの部分木DPを merge する演算
    Node operator *(const Node &x) const {
        return {dist_sum + x.dist_sum, subtree_size + x.subtree_size};
    }
    Node operator *=(const Node &x) {
        dist_sum += x.dist_sum;
        subtree_size += x.subtree_size;
        return *this;
    }
    // 子の木DP結果を利用する演算
    Node operator *() const {
        return {dist_sum + subtree_size + 1, subtree_size + 1};
    }
};

class RerootingTreeDP {
private:
    std::vector<std::vector<int>> _graph;
    std::vector<std::vector<Node>> _dp;  // 頂点 v の i 番目の部分木について
    std::vector<Node> _answer;

    Node dp(const int v, const int parent) {
        auto a = Node::e();
        _dp[v].resize(_graph[v].size());
        for (size_t i = 0; i < _graph[v].size(); ++i)
            if (_graph[v][i] != parent)
                a *= *(_dp[v][i] = dp(_graph[v][i], v));
        return a;
    }
    void rerooting(const int v, const int parent, const Node &part) {
        if (auto i = std::find(_graph[v].begin(), _graph[v].end(), parent); i != _graph[v].end())
            _dp[v][i - _graph[v].begin()] = *part;
        auto before = std::vector(_graph[v].size(), Node::e());
        auto after = std::vector(_graph[v].size(), Node::e());
        for (size_t i = 1; i < _dp[v].size(); ++i) {
            size_t j = _dp[v].size() - i;
            before[i] = before[i - 1] * _dp[v][i - 1];
            after[j - 1] = after[j] * _dp[v][j];
        }
        for (size_t i = 0; i < _graph[v].size(); ++i)
            if (int u = _graph[v][i]; u != parent)
                rerooting(_graph[v][i], v, before[i] * after[i]);
    }
public:
    explicit RerootingTreeDP(std::vector<std::vector<int>> &&graph)
    : _graph(std::move(graph)), _dp(_graph.size()), _answer(_graph.size())
    {}
};