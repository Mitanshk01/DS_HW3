#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

bool check_bound(double &z_real, double &z_img, double c_real, double c_img)
{
  double z_next_real = (z_real * z_real) - (z_img * z_img) + c_real;
  double z_next_img = (2 * z_real * z_img) + c_img;
  double mag = (z_next_real * z_next_real) + (z_next_img * z_next_img);
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
  z_img -= 1.5;
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
  int *counts_gather = (int *)malloc(sizeof(int) * size);
  int *displacements_gather = (int *)malloc(sizeof(int) * size);
  pair<int, int> *indices;
  pair<int, int> *rec_indices;
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
  indices = (pair<int, int> *)malloc(sizeof(pair<int, int>) * (n * m));

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
    counts[i] *= 2;
    displacements[i] = cnt_total;
    cnt_total += counts[i];
  }

  cnt_total = 0;
  for (int i = 0; i < size; i++)
  {
    counts_gather[i] = total / size;
    int rem = total % size;
    if (rem > i)
    {
      counts_gather[i]++;
    }
    displacements_gather[i] = cnt_total;
    cnt_total += counts_gather[i];
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      indices[i * n + j].first = i + 1;
      indices[i * n + j].second = j + 1;
    }
  }

  rec_indices = (pair<int, int> *)malloc(sizeof(pair<int, int>) * counts[rank]);
  temp_ans = (int *)malloc(sizeof(int) * (counts[rank] / 2));

  MPI_Scatterv(indices, counts, displacements, MPI_INT, rec_indices, counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < counts[rank] / 2; i++)
  {
    double z0_real = get_real(m, rec_indices[i].second);
    double z0_img = get_img(n, rec_indices[i].first);
    temp_ans[i] = 1;
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

  MPI_Gatherv(temp_ans, counts_gather[rank], MPI_INT, ans, counts_gather, displacements_gather, MPI_INT, 0, MPI_COMM_WORLD);

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < m; j++)
      {
        cout << ans[i * n + j] << ' ';
      }
      cout << endl;
    }
    // cout<<"Total time taken(s) : "<<total_time<<"\n";
  }

  MPI_Finalize();
  return 0;
}