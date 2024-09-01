#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

#define int long long

int minMultRec(const std::vector<int> &arr, int i, int j, std::vector<std::vector<int>> &memo)
{
  if (i + 1 == j)
    return 0;

  if (memo[i][j] != -1)
    return memo[i][j];

  int res = std::numeric_limits<int>::max();

  for (int k = i + 1; k < j; k++)
  {
    int curr = minMultRec(arr, i, k, memo) +
               minMultRec(arr, k, j, memo) +
               arr[i] * arr[k] * arr[j];
    res = std::min(curr, res);
  }

  memo[i][j] = res;
  return res;
}

int minMult(const std::vector<int> &arr)
{
  int n = arr.size();
  std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
  return minMultRec(arr, 0, n - 1, memo);
}

signed main(int argc, char *argv[])
{
  std::ifstream infile(argv[1]);
  int N;
  infile >> N;
  std::vector<int> arr(N + 1);
  for (int i = 0; i <= N; ++i)
  {
    infile >> arr[i];
  }
  infile.close();

  std::cout << minMult(arr) << std::endl;
  return 0;
}
