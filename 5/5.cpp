#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

typedef long long ll;

int main(int argc, char *argv[])
{
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n;
  int *mat;
  ll **dp;
  ll *local_results;
  ll *gathered_results;
  const char *filename = argv[1];

  if (rank == 0)
  {
    freopen(filename, "r", stdin);
    cin >> n;
    n++;
  }
  double start_time = MPI_Wtime();
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  mat = (int *)malloc(sizeof(int) * n);

  if (rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      cin >> mat[i];
    }
    fclose(stdin);
  }

  MPI_Bcast(mat, n, MPI_INT, 0, MPI_COMM_WORLD);

  dp = (ll **)malloc(sizeof(ll *) * n);
  for (int i = 0; i < n; i++)
  {
    dp[i] = (ll *)malloc(sizeof(ll) * n);
    for (int j = 0; j < n; j++)
    {
      dp[i][j] = 0;
    }
  }

  for (int slider_len = 2; slider_len < n; slider_len++)
  {
    int block_size = (n - slider_len) / size;
    int remainder = (n - slider_len) % size;

    int st = rank * block_size + min(rank, remainder);
    int en = st + block_size + (rank < remainder ? 1 : 0);

    local_results = (ll *)malloc(sizeof(ll) * (en - st));
    gathered_results = (ll *)malloc(sizeof(ll) * (n - slider_len));

    int *counts = (int *)malloc(size * sizeof(int));
    int *displacements = (int *)malloc(size * sizeof(int));
    int cnt_total = 0;
    int total = n - slider_len;

    for (int i = 0; i < size; i++)
    {
      counts[i] = total / size;
      int rem = total % size;
      if (rem > i)
      {
        counts[i]++;
      }
      displacements[i] = cnt_total;
      cnt_total += counts[i];
    }

    for (int i = st; i < en; i++)
    {
      int j = i + slider_len;
      dp[i][j] = LONG_LONG_MAX;
      for (int k = i + 1; k < j; k++)
      {
        dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j] + (ll)mat[i] * mat[k] * mat[j]);
      }
      local_results[i - st] = dp[i][j];
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allgatherv(local_results, en - st, MPI_LONG_LONG, gathered_results, counts, displacements, MPI_LONG_LONG, MPI_COMM_WORLD);

    for (int i = 0; i < n - slider_len; i++)
    {
      int j = i + slider_len;
      dp[i][j] = gathered_results[i];
    }

    free(local_results);
    free(gathered_results);
  }

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    cout << dp[0][n - 1] << endl;
    // cout << "Total time taken(s) : " << total_time << "\n";
  }

  MPI_Finalize();
  return 0;
}
