#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

void generate_test_case(int test_num, int N, int M, int K)
{
    std::ofstream outfile("test_cases/test" + std::to_string(test_num) + ".txt");
    outfile << N << " " << M << " " << K << std::endl;

    // Generate N points
    for (int i = 0; i < N; ++i)
    {
        double x = (std::rand() % 100000) / 100.0;
        double y = (std::rand() % 100000) / 100.0;
        outfile << std::fixed << std::setprecision(2) << x << " " << y << std::endl;
    }

    // Generate M query points
    for (int i = 0; i < M; ++i)
    {
        double x = (std::rand() % 100000) / 100.0;
        double y = (std::rand() % 100000) / 100.0;
        outfile << std::fixed << std::setprecision(2) << x << " " << y << std::endl;
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
        int N = 1 + std::rand() % 10; // N between 1 and 10
        int M = 1 + std::rand() % 10; // M between 1 and 10
        int K = 1 + std::rand() % N;  // K between 1 and N
        generate_test_case(i, N, M, K);
    }

    // Generate 10 larger test cases with constraint Q*K <= 200,000
    for (int i = 6; i <= 15; ++i)
    {
        int N = 1 + std::rand() % 10000; // N between 1 and 10,000

        // Ensure Q * K <= 200,000
        int Q = 1 + std::rand() % 10000; // M between 1 and 10,000
        int K = 1 + std::min(N, static_cast<int>(200000 / Q)); // K between 1 and min(N, 200,000 / Q)

        generate_test_case(i, N, Q, K);
    }

    return 0;
}
