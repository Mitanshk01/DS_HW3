#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

void generate_test_case(int test_num, int N)
{
    std::ofstream outfile("test_cases/test" + std::to_string(test_num) + ".txt");
    outfile << N << std::endl;
    for (int i = 0; i < N; ++i)
    {
        double num = (std::rand() % 100000) / 100.0; // Generates a number with at most 2 decimal places
        outfile << std::fixed << std::setprecision(2) << num;
        if (i != N - 1)
            outfile << " ";
    }
    outfile << std::endl;
    outfile.close();
}

int main()
{
    std::srand(std::time(0));
    std::system("mkdir -p test_cases");

    // Generate 5 very small test cases with N <= 10
    for (int i = 1; i <= 5; ++i)
    {
        int N = 1 + std::rand() % 10; // Generate N between 1 and 10
        generate_test_case(i, N);
    }

    // Generate the remaining 10 test cases with N between 1 and 100,000
    for (int i = 6; i <= 15; ++i)
    {
        int N = 1 + std::rand() % 5000000; // Gene rate N between 1 and 200,000
        generate_test_case(i, N);
    }

    return 0;
}
