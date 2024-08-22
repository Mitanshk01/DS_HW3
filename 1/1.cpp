#include <bits/stdc++.h>
using namespace std;
#include <mpi.h>

int main(){
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size = 0;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Variable Declarations
  int n, m, k;
  double **p; // points
  double **q; // queries

  if(rank == 0){
    cin>>n>>m>>k;
  }

  double start_time = MPI_Wtime();

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);  

  p = (double**)malloc(sizeof(double*)*n);
  for(int i=0;i<n;i++){
    p[i]=(double*)malloc(sizeof(double)*2);
  }

  q = (double**)malloc(sizeof(double*)*m);
  for(int i=0;i<m;i++){
    q[i]=(double*)malloc(sizeof(double)*2);
  }

  if(rank == 0){
    for(int i=0;i<n;i++){
      cin>>p[i][0]>>p[i][1];
    }
    for(int i=0;i<m;i++){
      cin>>q[i][0]>>q[i][1];
    }
  }

  for(int i=0;i<m;i++){
    MPI_Bcast(q[i], 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  
}