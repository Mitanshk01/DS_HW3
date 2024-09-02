#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void generate_test_case(int test_num, int N, int M, int K, double c_r, double c_i)
{
  ofstream outfile("test_cases/test" + to_string(test_num) + ".txt");
  outfile << N << " " << M << " " << K << endl;
  outfile << c_r << " " << c_i << endl;
  outfile.close();
}

int main()
{
  srand(time(0));
  system("mkdir -p test_cases");

  for (int i = 1; i <= 15; i++)
  {
    int N = rand() % 999 + 2;                                      // 2 ≤ N ≤ 1000
    int M = rand() % 999 + 2;                                      // 2 ≤ M ≤ 1000
    int K = rand() % 100 + 1;                                     // 1 ≤ K ≤ 100
    double c_r = -2 + static_cast<double>(rand()) / RAND_MAX * 4; // -2 ≤ c_r ≤ 2
    double c_i = -2 + static_cast<double>(rand()) / RAND_MAX * 4; // -2 ≤ c_i ≤ 2

    generate_test_case(i, N, M, K, c_r, c_i);
  }

  return 0;
}
