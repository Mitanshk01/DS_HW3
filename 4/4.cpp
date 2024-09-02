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

  int n;
  double **mat;
  double **I;
  const char *filename = argv[1];
  if (rank == 0)
  {
    freopen(filename, "r", stdin);
    cin >> n;
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  mat = (double **)malloc(sizeof(double *) * n);
  I = (double **)malloc(sizeof(double *) * n);
  for (int i = 0; i < n; i++)
  {
    mat[i] = (double *)malloc(sizeof(double) * n);
    I[i] = (double *)malloc(sizeof(double) * n);
    for (int j = 0; j < n; j++)
    {
      I[i][j] = 0;
    }
    I[i][i] = 1;
  }

  if (rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        cin >> mat[i][j];
      }
    }
    fclose(stdin);
  }

  for (int i = 0; i < n; i++)
  {
    MPI_Bcast(mat[i], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  double start_time = MPI_Wtime();

  for (int i = 0; i < n; i++)
  {
    int swap_row = 0;
    for (int j = i + 1; j < n; j++)
    {
      if (j % size == rank && mat[j][i] != 0)
      {
        swap_row = j;
        break;
      }
    }

    int global_swap_row;
    MPI_Allreduce(&swap_row, &global_swap_row, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    MPI_Bcast(mat[global_swap_row], n, MPI_DOUBLE, global_swap_row % size, MPI_COMM_WORLD);
    MPI_Bcast(I[global_swap_row], n, MPI_DOUBLE, global_swap_row % size, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == i % size)
    {
      if (mat[i][i] == 0)
      {
        for (int col = 0; col < n; col++)
        {
          swap(mat[i][col], mat[global_swap_row][col]);
          swap(I[i][col], I[global_swap_row][col]);
        }
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(mat[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    MPI_Bcast(mat[global_swap_row], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    MPI_Bcast(I[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    MPI_Bcast(I[global_swap_row], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);

    double pivot_val = mat[i][i];
    MPI_Bcast(&pivot_val, 1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);

    if (rank == i % size)
    {
      for (int col = 0; col < n; col++)
      {
        mat[i][col] /= pivot_val;
        I[i][col] /= pivot_val;
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(mat[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    MPI_Bcast(I[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);

    for (int row = i + 1; row < n; row++)
    {
      if (row % size == rank)
      {
        double row_val = mat[row][i];
        for (int col = 0; col < n; col++)
        {
          mat[row][col] -= row_val * mat[i][col];
          I[row][col] -= row_val * I[i][col];
        }
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  for (int i = n - 1; i >= 0; i--)
  {
    MPI_Bcast(mat[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    MPI_Bcast(I[i], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    for (int row = i - 1; row >= 0; row--)
    {
      if (row % size == rank)
      {
        double row_val = mat[row][i];
        for (int col = 0; col < n; col++)
        {
          mat[row][col] -= row_val * mat[i][col];
          I[row][col] -= row_val * I[i][col];
        }
      }
    }
  }

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    // cout<<"Total time taken(s) : "<<total_time<<"\n";
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        cout << fixed << setprecision(2) << I[i][j] << ' ';
      }
      cout << endl;
    }
  }

  MPI_Finalize();
  return 0;
}