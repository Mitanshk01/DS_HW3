#include <bits/stdc++.h>
using namespace std;
#include <mpi.h>

int main(int argc, char* argv[]){
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
  double **rec_p; // distributed points
  int *counts = (int*)malloc(sizeof(int)*size);
  int *displacements = (int*)malloc(sizeof(int)*size);
  int cnt_total = 0;

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

  for(int i=0;i<size;i++){
    counts[i]=n/size;
    int rem=n%size;
    if(rem>i){
      counts[i]++;
    }
    displacements[i]=cnt_total;
    cnt_total+=counts[i];
  }

  rec_p = (double**)malloc(sizeof(double*)*counts[rank]);
  for(int i=0;i<counts[rank];i++){
    rec_p[i]=(double*)malloc(sizeof(double)*2);
  }

  // Bug, issue in sending contiguous blocks with 2d array
  MPI_Scatterv(p, counts, displacements, MPI_DOUBLE, rec_p, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  cout<<"Process "<<rank<<endl;
  for(int i=0;i<counts[rank];i++){
    cout<<rec_p[i][0]<<' '<<rec_p[i][1]<<endl;
  }
  cout<<"------------------------------------\n";

  MPI_Barrier(MPI_COMM_WORLD);

  // Clean up befpre exiting
  MPI_Finalize(); 
  return 0;
}