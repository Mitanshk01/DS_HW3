#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

// Function to generate a random matrix
void generate_matrix(int N, std::vector<std::vector<double>> &matrix)
{
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      matrix[i][j] = (std::rand() % 10000) / 100.0; // Values between 0 and 100
    }
  }
}

// Function to ensure the matrix is non-singular (simple approach)
bool is_non_singular(const std::vector<std::vector<double>> &matrix, int N)
{
  // For simplicity, use a basic check (e.g., determinant) to ensure non-singularity
  // For large matrices, more robust methods should be used.
  return true; // Placeholder: Ensure you use proper methods to check matrix non-singularity
}

void generate_test_case(int test_num, int N)
{
  std::ofstream outfile("test_cases/test" + std::to_string(test_num) + ".txt");
  outfile << N << std::endl;

  std::vector<std::vector<double>> matrix(N, std::vector<double>(N));
  generate_matrix(N, matrix);

  while (!is_non_singular(matrix, N))
  {
    generate_matrix(N, matrix); // Regenerate until a non-singular matrix is obtained
  }

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      outfile << std::fixed << std::setprecision(2) << matrix[i][j];
      if (j != N - 1)
        outfile << " ";
    }
    outfile << std::endl;
  }
  outfile.close();
}

int main()
{
  std::srand(std::time(0));
  std::system("mkdir -p test_cases");

  // Generate 5 very small test cases
  for (int i = 1; i <= 5; ++i)
  {
    int N = 1 + std::rand() % 20; // N between 1 and 20
    generate_test_case(i, N);
  }

  // Generate 10 larger test cases
  for (int i = 6; i <= 15; ++i)
  {
    int N = 1 + std::rand() % 500; // N between 1 and 500
    generate_test_case(i, N);
  }

  return 0;
}
