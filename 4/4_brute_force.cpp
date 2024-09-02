#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

// Function to perform Gaussian elimination and find the inverse of the matrix
bool invert_matrix(const std::vector<std::vector<double>> &A, std::vector<std::vector<double>> &inv, int N)
{
  std::vector<std::vector<double>> mat(N, std::vector<double>(2 * N));

  // Initialize augmented matrix [A | I]
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      mat[i][j] = A[i][j];
      mat[i][j + N] = (i == j) ? 1 : 0;
    }
  }

  // Apply Gaussian elimination
  for (int i = 0; i < N; ++i)
  {
    // Search for maximum in this column
    double maxEl = std::abs(mat[i][i]);
    int maxRow = i;
    for (int k = i + 1; k < N; ++k)
    {
      if (std::abs(mat[k][i]) > maxEl)
      {
        maxEl = std::abs(mat[k][i]);
        maxRow = k;
      }
    }

    // Swap maximum row with current row
    std::swap(mat[maxRow], mat[i]);

    // Make all rows below this one 0 in current column
    for (int k = i + 1; k < N; ++k)
    {
      double c = -mat[k][i] / mat[i][i];
      for (int j = i; j < 2 * N; ++j)
      {
        mat[k][j] += c * mat[i][j];
      }
    }
  }

  // Solve equation for an upper triangular matrix
  for (int i = N - 1; i >= 0; --i)
  {
    for (int j = N; j < 2 * N; ++j)
    {
      mat[i][j] /= mat[i][i];
    }
    for (int k = i - 1; k >= 0; --k)
    {
      double c = -mat[k][i];
      for (int j = i; j < 2 * N; ++j)
      {
        mat[k][j] += c * mat[i][j];
      }
    }
  }

  // Extract the inverse matrix
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      inv[i][j] = mat[i][j + N];
    }
  }

  return true;
}

int main(int argc, char *argv[])
{
  std::ifstream infile(argv[1]);
  int N;
  infile >> N;

  std::vector<std::vector<double>> A(N, std::vector<double>(N));
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      infile >> A[i][j];
    }
  }
  infile.close();

  std::vector<std::vector<double>> inv(N, std::vector<double>(N));
  if (invert_matrix(A, inv, N))
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        std::cout << std::fixed << std::setprecision(2) << inv[i][j];
        if (j != N - 1)
          std::cout << " ";
      }
      std::cout << std::endl;
    }
  }
  else
  {
    std::cerr << "Matrix is singular and cannot be inverted." << std::endl;
    return 1;
  }

  return 0;
}
