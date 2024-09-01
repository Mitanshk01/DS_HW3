#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

double get_magnitude(double z_real, double z_img)
{
  double mag = (z_real * z_real) + (z_img * z_img);
  return mag;
}

bool check_bound(double &z_real, double &z_img, double c_real, double c_img)
{
  double z_next_real = (z_real * z_real) - (z_img * z_img) + c_real;
  double z_next_img = (2 * z_real * z_img) + c_img;
  double mag = get_magnitude(z_next_real, z_next_img);
  z_real = z_next_real;
  z_img = z_next_img;
  if (mag > 4)
  {
    return false;
  }
  return true;
}

double get_real(int m, int ind_real)
{
  double num = 3 * (ind_real - 1);
  double denom = m - 1;
  double z_real = num / denom;
  z_real -= 1.5;
  return z_real;
}

double get_img(int n, int ind_img)
{
  double num = 3 * (ind_img - 1);
  double denom = n - 1;
  double z_img = num / denom;
  z_img = 1.5 - z_img;
  return z_img;
}

int main(int argc, char *argv[])
{
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n, m, k;
  double c_real, c_img;
  int *ans;
  int *temp_ans;
  int *counts = (int *)malloc(sizeof(int) * size);
  int *displacements = (int *)malloc(sizeof(int) * size);
  const char *filename = argv[1];

  if (rank == 0)
  {
    freopen(filename, "r", stdin);
    cin >> n >> m >> k;
    cin >> c_real >> c_img;
    fclose(stdin);
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&c_real, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&c_img, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  double start_time = MPI_Wtime();

  ans = (int *)malloc(sizeof(int) * (n * m));

  int cnt_total = 0;
  int total = n * m;

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

  temp_ans = (int *)malloc(sizeof(int) * (counts[rank]));

  int proc_st_idx = displacements[rank];
  int proc_end_idx = proc_st_idx + counts[rank];
  // cout<<"Process "<<rank<<' '<<proc_st_idx<<' '<<proc_end_idx<<endl;
  for (int i = 0; i < counts[rank]; i++)
  {
    int glob_idx = proc_st_idx + i;
    int rval = glob_idx % m;
    rval++;
    int ival = glob_idx / m;
    ival++;
    double z0_real = get_real(m, rval);
    double z0_img = get_img(n, ival);
    temp_ans[i] = 1;
    if (get_magnitude(z0_real, z0_img) > 4)
    {
      temp_ans[i] = 0;
      continue;
    }
    for (int j = 1; j <= k; j++)
    {
      bool ok = check_bound(z0_real, z0_img, c_real, c_img);
      if (!ok)
      {
        temp_ans[i] = 0;
        break;
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Gatherv(temp_ans, counts[rank], MPI_INT, ans, counts, displacements, MPI_INT, 0, MPI_COMM_WORLD);

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    for (int i = n - 1; i >= 0; i--)
    {
      for (int j = 0; j < m; j++)
      {
        cout << ans[i * m + j] << ' ';
      }
      cout << endl;
    }
    // cout<<"Total time taken(s) : "<<total_time<<"\n";
  }

  MPI_Finalize();
  return 0;
}