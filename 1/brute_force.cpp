#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>

// Function to calculate squared Euclidean distance
double euclidean_distance_squared(const std::pair<double, double> &p1, const std::pair<double, double> &p2)
{
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

int main(int argc, char *argv[])
{
    std::ifstream infile(argv[1]);
    int N, M, K;
    infile >> N >> M >> K;

    std::vector<std::pair<double, double>> points(N);
    std::vector<std::pair<double, double>> queries(M);

    // Read points
    for (int i = 0; i < N; ++i)
    {
        infile >> points[i].first >> points[i].second;
    }

    // Read queries
    for (int i = 0; i < M; ++i)
    {
        infile >> queries[i].first >> queries[i].second;
    }
    infile.close();

    for (const auto &query : queries)
    {
        std::vector<std::pair<double, std::pair<double, double>>> distances;
        for (const auto &point : points)
        {
            double dist = euclidean_distance_squared(query, point);
            distances.emplace_back(dist, point);
        }

        // Sort distances based on the first element (distance squared)
        std::sort(distances.begin(), distances.end());

        for (int i = 0; i < K; ++i)
        {
            const auto &point = distances[i].second;
            std::cout << std::fixed << std::setprecision(2) << point.first << " " << point.second << std::endl;
        }
    }

    return 0;
}
