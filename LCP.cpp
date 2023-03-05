/*#include <iostream>
#include <algorithm>

void LCP(int32_t* x, int32_t* y, int32_t** dp, int32_t n, int32_t m) {
  for (int32_t i = 0; i < n; ++i) {
    for (int32_t j = 0; j < m; ++j) {
      if (x[i] == y[j]) {
        dp[i + 1][j + 1] = dp[i][j] + 1;
      } else {
        dp[i + 1][j + 1] = std::max(dp[i][j + 1], dp[i + 1][j]);
      }
    }
  }
}

void RestoreSubsequence(int32_t* x, int32_t** dp, int32_t n, int32_t m) {
  if (n == 0 || m == 0) {
    return;
  }
  if (dp[n][m] == dp[n - 1][m]) {
    RestoreSubsequence(x, dp, n - 1, m);
    return;
  }
  if (dp[n][m] == dp[n][m - 1]) {
    RestoreSubsequence(x, dp, n, m - 1);
    return;
  }
  if (dp[n][m] == dp[n - 1][m - 1] + 1) {
    RestoreSubsequence(x, dp, n - 1, m - 1);
    std::cout << x[n - 1] << ' ';
    return;
  }
}

int main() {
  int32_t n;
  int32_t m;
  std::cin >> n;
  auto x = new int32_t[n];
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> x[i];
  }
  std::cin >> m;
  auto y = new int32_t[m];
  for (int32_t i = 0; i < m; ++i) {
    std::cin >> y[i];
  }
  auto dp = new int32_t*[n + 1];
  for (int32_t i = 0; i <= n; ++i) {
    dp[i] = new int[m + 1];
  }
  for (int32_t i = 0; i <= n; ++i) {
    for (int32_t j = 0; j <= m; ++j) {
      dp[i][j] = 0;
    }
  }
  LCP(x, y, dp, n, m);
  RestoreSubsequence(x, dp, n, m);
  for (int32_t i = 0; i <= n; ++i) {
    delete[] dp[i];
  }
  delete[] dp;
  delete[] x;
  delete[] y;
  return 0;
}
*/
