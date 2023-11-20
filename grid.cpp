#include <utility>
#include <array>
using namespace std;

int f() {
    int H, W;

    // 右方向から反時計回り
    // note: 上からi, 左からj番目のマス
    constexpr array<pair<int, int>, 4> didj4 {
        {{0, 1}, {1, 0}, {0, -1}, {-1, 0},}
    };
    constexpr array<pair<int, int>, 8> didj8 {
        {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1},}
    };

    const auto check_index = [&](int i, int j){
        return 0 <= i && i < H && 0 <= j && j < W;
    };
    const auto serialize_index = [&](int i, int j){
        return i * W + j;
    };
}
