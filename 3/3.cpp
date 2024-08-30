#include <bits/stdc++.h>
using namespace std;
#include <mpi.h>

typedef long long ll;

int main(int argc, char *argv[])
{
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size = 0;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Variable Declarations
  int n;
  double *p;
  double *rec_p;
  double *answer_pref;
  double sum = 0;
  int *counts = (int *)malloc(sizeof(int) * size);
  int *displacements = (int *)malloc(sizeof(int) * size);
  int cnt_total = 0;
  double prefix_add = 0;
  const char *filename = argv[1];

  if (rank == 0)
  {
    freopen(filename, "r", stdin);
    cin >> n;
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  p = (double *)malloc(sizeof(double) * n);
  answer_pref = (double *)malloc(sizeof(double) * n);
  if (rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      cin >> p[i];
    }
    fclose(stdin);
  }

  for (int i = 0; i < size; i++)
  {
    counts[i] = n / size;
    int rem = n % size;
    if (rem > i)
    {
      counts[i]++;
    }
    displacements[i] = cnt_total;
    cnt_total += counts[i];
  }

  rec_p = (double *)malloc(sizeof(double) * counts[rank]);
  MPI_Scatterv(p, counts, displacements, MPI_DOUBLE, rec_p, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double start_time = MPI_Wtime();

  for (int i = 1; i < counts[rank]; i++)
  {
    rec_p[i] += rec_p[i - 1];
  }

  sum = rec_p[counts[rank] - 1];

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank != 0)
  {
    MPI_Recv(&prefix_add, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  if (rank != size - 1)
  {
    double temp_sum = sum + prefix_add;
    MPI_Send(&temp_sum, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  for (int i = 0; i < counts[rank]; i++)
  {
    rec_p[i] += prefix_add;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gatherv(rec_p, counts[rank], MPI_DOUBLE, answer_pref, counts, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    cout << "Total time taken(s) : " << total_time << "\n";
    for (int i = 0; i < n; i++)
    {
      cout << fixed << setprecision(2) << answer_pref[i] << ' ';
    }
    cout << endl;
  }

  // Clean up befpre exiting
  MPI_Finalize();
  return 0;
}
