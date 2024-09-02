#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

void square_complex_number(double *real_pointer, double *imag_pointer)
{
  double a = *real_pointer;
  double b = *imag_pointer;

  *real_pointer = a * a - b * b;
  *imag_pointer = 2 * a * b;
}

void julia_next_iteration(double *real_pointer, double *imag_pointer, double c_r, double c_i)
{
  square_complex_number(real_pointer, imag_pointer);
  *real_pointer = *real_pointer + c_r;
  *imag_pointer = *imag_pointer + c_i;
}

bool is_magnitude_under_thresh(double real, double imag, double T)
{
  return (real * real + imag * imag) <= T * T;
}

int compute_for_point(double real, double imag, int K, double T, double c_r, double c_i)
{
  for (int i = 0; i < K; i++)
  {
    if (!is_magnitude_under_thresh(real, imag, T))
    {
      return 0;
    }
    julia_next_iteration(&real, &imag, c_r, c_i);
  }
  return is_magnitude_under_thresh(real, imag, T) ? 1 : 0;
}

void generate_julia_set(int N, int M, int K, double c_r, double c_i)
{
  double T = 2.0;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++)
    {
      double real = -1.5 + ((double)j * (double)((double)3.0 / (double)(M - 1)));
      double imag = 1.5 - ((double)i * (double)((double)3.0 / (double)(N - 1)));
      cout << compute_for_point(real, imag, K, T, c_r, c_i) << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[])
{
  ifstream infile(argv[1]);
  int N, M, K;
  double c_r, c_i;

  infile >> N >> M >> K;
  infile >> c_r >> c_i;

  generate_julia_set(N, M, K, c_r, c_i);

  return 0;
}
