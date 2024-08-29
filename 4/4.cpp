#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n;
  double **mat;

  if(rank == 0){
    cin>>n;
  }
  double start_time = MPI_Wtime();
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 

  mat = (double**)malloc(sizeof(double*)*n);
  for(int i=0;i<n;i++){
    mat[i]=(double*)malloc(sizeof(double)*n);
  }

  if(rank == 0){
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        cin>>mat[i][j];
      }
    }
  }

  

  MPI_Finalize(); 
  return 0;
}