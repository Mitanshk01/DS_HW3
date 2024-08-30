#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

typedef long long ll;

int main(int argc, char* argv[]) {
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n;
  int *mat;
  ll **dp;  // Change to ll

  if(rank == 0){
    cin >> n; n++;
  }
  double start_time = MPI_Wtime();
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 

  mat = (int*)malloc(sizeof(int) * n);

  if(rank == 0){
    for(int i = 0; i < n; i++){
      cin >> mat[i];
    }
  }

  MPI_Bcast(mat, n, MPI_INT, 0, MPI_COMM_WORLD);

  dp = (ll**)malloc(sizeof(ll*) * n); 
  for(int i = 0; i < n; i++){
    dp[i] = (ll*)malloc(sizeof(ll) * n); 
    for(int j = 0; j < n; j++){
      dp[i][j] = 0;
    }
  }
  
  for(int slider_len = 2; slider_len < n; slider_len++){
    for(int i = 0; i < n - slider_len; i++){
      int j = i + slider_len;
      dp[i][j] = LLONG_MAX;  

      ll local_min_cost = LLONG_MAX;

      int k_start = i + 1 + rank * ((j - i - 1) / size);
      int k_end = i + 1 + (rank + 1) * ((j - i - 1) / size);
      if (rank == size - 1) {
          k_end = j; 
      }

      for (int k = k_start; k < k_end; k++) {
          ll cost = dp[i][k] + dp[k][j] + (ll)mat[i] * mat[k] * mat[j];
          local_min_cost = min(local_min_cost, cost);
      }

      MPI_Barrier(MPI_COMM_WORLD);
      ll global_min_cost;
      MPI_Allreduce(&local_min_cost, &global_min_cost, 1, MPI_LONG_LONG, MPI_MIN, MPI_COMM_WORLD);
      dp[i][j] = global_min_cost;
    }
  }

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    cout << dp[0][n-1] << endl;
    cout<<"Total time taken(s) : "<<total_time<<"\n";
  }

  MPI_Finalize(); 
  return 0;
}
