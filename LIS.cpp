/*#include <iostream>
#include <algorithm>

void LIS(int32_t* x, int32_t* dp, int32_t size) {
  const int32_t inf = 1000000;
  dp[0] = -inf;
  for (int32_t i = 1; i < size; ++i) {
    dp[i] = inf;
  }
  for (int32_t i = 0; i < size; ++i) {
    auto j = std::lower_bound(dp, dp + size, x[i]);
    if (*j > x[i]) {
      *j = x[i];
    }
  }
}

int main() {
  int32_t n;
  std::cin >> n;
  auto x = new int32_t[n];
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> x[i];
  }
  auto dp = new int32_t[n];
  LIS(x, dp, n);
  for (int32_t i = 0; i < n; ++i) {
    if (dp[i] == 1000000) {
      std::cout << i - 1;
      break;
    }
  }
  delete[] x;
  delete[] dp;
  return 0;
}
*/
