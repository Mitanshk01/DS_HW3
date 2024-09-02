#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

void generate_test_case(int test_num, int N, int max_dim)
{
  std::ofstream outfile("test_cases/test" + std::to_string(test_num) + ".txt");
  outfile << N << std::endl;
  std::vector<int> dims(N + 1);
  for (int i = 0; i <= N; ++i)
  {
    dims[i] = 1 + std::rand() % max_dim; // Dimensions between 1 and max_dim
  }
  for (int i = 0; i <= N; ++i)
  {
    outfile << dims[i];
    if (i != N)
      outfile << " ";
  }
  outfile << std::endl;
  outfile.close();
}

int main()
{
  std::srand(std::time(0));
  std::system("mkdir -p test_cases");

  // Generate 5 very small test cases
  for (int i = 1; i <= 5; ++i)
  {
    int N = 1 + std::rand() % 10; // N between 1 and 10
    generate_test_case(i, N, 10);
  }

  // Generate 10 larger test cases
  for (int i = 6; i <= 15; ++i)
  {
    int N = 1 + std::rand() % 500; // N between 1 and 500
    generate_test_case(i, N, 1000);
  }

  return 0;
}
