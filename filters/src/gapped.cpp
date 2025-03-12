#include "gapped.hpp"

namespace filters {
void gappedCompute(std::vector<filters::Hit> &hits, const std::string &query,
                   const std::string &reference) {
  // printf("gappedCompute\n");
  for (auto &hit : hits) {
    if (hit.on) {
      gappedFilter(hit, query, reference);
    }
  }
}

const int MATCH = 2;
const int MISMATCH = -1;
const int GAP_OPEN = -2;
const int GAP_EXTEND = -1;

void gappedFilter(Hit &hit, const std::string &query,
                  const std::string &reference) {
  int m = query.size();
  int n = reference.size();

  // Smith-Waterman DP table
  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
  std::vector<std::vector<int>> trace(m + 1, std::vector<int>(n + 1, 0));

  int max_score = 0, max_i = 0, max_j = 0;

  // Fill DP table
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      int match = dp[i - 1][j - 1] +
                  (query[i - 1] == reference[j - 1] ? MATCH : MISMATCH);
      int gap_x = dp[i - 1][j] + GAP_EXTEND;
      int gap_y = dp[i][j - 1] + GAP_EXTEND;

      dp[i][j] = std::max({0, match, gap_x, gap_y});

      if (dp[i][j] > max_score) {
        max_score = dp[i][j];
        max_i = i;
        max_j = j;
      }
    }
  }

  // Backtrack to find the best alignment start
  int i = max_i, j = max_j;
  while (i > 0 && j > 0 && dp[i][j] > 0) {
    if (dp[i][j] == dp[i - 1][j - 1] +
                        (query[i - 1] == reference[j - 1] ? MATCH : MISMATCH)) {
      --i;
      --j;
    } else if (dp[i][j] == dp[i - 1][j] + GAP_EXTEND) {
      --i;
    } else {
      --j;
    }
  }

  // Update hit with the best extended alignment
  hit.position_x = i;
  hit.position_y = j;
  hit.length = std::max(max_i - i, max_j - j);
}

} // namespace filters