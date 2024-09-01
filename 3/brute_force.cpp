#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

int main(int argc, char *argv[])
{
  std::ifstream infile(argv[1]);
  int N;
  infile >> N;

  std::vector<double> a(N), p(N);
  for (int i = 0; i < N; ++i)
  {
    infile >> a[i];
  }
  infile.close();

  p[0] = a[0];
  for (int i = 1; i < N; ++i)
  {
    p[i] = p[i - 1] + a[i];
  }

  for (int i = 0; i < N; ++i)
  {
    std::cout << std::fixed << std::setprecision(2) << p[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}
