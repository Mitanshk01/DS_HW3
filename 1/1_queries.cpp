#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Variable Declarations
  int n, m, k;
  pair<double, double> *p;     // Points
  pair<double, double> *q;     // Queries
  pair<double, double> *rec_q; // Received points
  pair<double, double> *ans_q; // Sorted points for every query in every proc
  pair<double, double> *all_q; // Sorted points for every query in every proc
  int *counts = (int *)malloc(sizeof(int) * size);
  int *displacements = (int *)malloc(sizeof(int) * size);
  int *counts_gather = (int *)malloc(sizeof(int) * size);
  int *displacements_gather = (int *)malloc(sizeof(int) * size);
  const char *filename = argv[1];

  if (rank == 0)
  {
    freopen(filename, "r", stdin);
    cin >> n >> m >> k;
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

  q = (pair<double, double> *)malloc(sizeof(pair<double, double>) * m);
  p = (pair<double, double> *)malloc(sizeof(pair<double, double>) * n);
  all_q = (pair<double, double> *)malloc(sizeof(pair<double, double>) * (m * k));

  if (rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      cin >> p[i].first >> p[i].second;
    }
    for (int i = 0; i < m; i++)
    {
      cin >> q[i].first >> q[i].second;
    }
    fclose(stdin);
  }

  double start_time = MPI_Wtime();
  MPI_Bcast(p, n * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  int cnt_total = 0;
  for (int i = 0; i < size; i++)
  {
    counts[i] = m / size;
    int rem = m % size;
    if (rem > i)
    {
      counts[i]++;
    }
    counts[i] *= 2;
    displacements[i] = cnt_total;
    counts_gather[i] = counts[i] * k;
    displacements_gather[i] = displacements[i] * k;
    cnt_total += counts[i];
  }

  rec_q = (pair<double, double> *)malloc(sizeof(pair<double, double>) * (counts[rank] / 2));
  ans_q = (pair<double, double> *)malloc(sizeof(pair<double, double>) * ((counts[rank] / 2) * k));

  MPI_Scatterv(q, counts, displacements, MPI_DOUBLE, rec_q, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < counts[rank] / 2; i++)
  {
    vector<pair<double, pair<double, double>>> dist_p;
    for (int j = 0; j < n; j++)
    {
      double dist = (rec_q[i].first - p[j].first) * (rec_q[i].first - p[j].first) + (rec_q[i].second - p[j].second) * (rec_q[i].second - p[j].second);
      dist_p.push_back({dist, p[j]});
    }
    sort(dist_p.begin(), dist_p.end());
    for (int j = 0; j < k; j++)
    {
      // cout << dist_p[j].second.first << ' ' << dist_p[j].second.second << endl;
      ans_q[i * k + j] = dist_p[j].second;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gatherv(ans_q, counts_gather[rank], MPI_DOUBLE, all_q, counts_gather, displacements_gather, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    // for (int i = 0; i < m * k; i++)
    // {
    //   cout << fixed << setprecision(2) << all_q[i].first << ' ' << all_q[i].second << endl;
    // }
    cout << "Total time taken(s) : " << total_time << "\n";
  }

  MPI_Finalize();
  return 0;
}