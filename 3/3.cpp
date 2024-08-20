#include <bits/stdc++.h>
using namespace std;
#include <mpi.h>

typedef long long ll;

int main(int argc, char* argv[])
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
    double *proc_sum_arr;
    double *answer_pref;
    double sum = 0;
    int *counts = (int*)malloc(sizeof(int)*size);
    int *displacements = (int*)malloc(sizeof(int)*size);
    int cnt_total = 0;
    double left_pref = 0;

    if(rank == 0){
      cin>>n;
    }

    double start_time = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    p = (double*)malloc(sizeof(double)*n);
    answer_pref = (double*)malloc(sizeof(double)*n);
    if(rank == 0){
      for(int i=0;i<n;i++){
        cin>>p[i];
      }
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

    rec_p = (double*)malloc(sizeof(double)*counts[rank]);
    MPI_Scatterv(p, counts, displacements, MPI_DOUBLE, rec_p, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i=1;i<counts[rank];i++){
      rec_p[i]+=rec_p[i-1];
    }

    sum = rec_p[counts[rank]-1];
    proc_sum_arr = (double*)malloc(sizeof(double)*size);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allgather(&sum, 1, MPI_DOUBLE, proc_sum_arr, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    for(int i=0;i<rank;i++){
      left_pref += proc_sum_arr[i];
    }

    for(int i=0;i<counts[rank];i++){
      rec_p[i] += left_pref;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gatherv(rec_p, counts[rank], MPI_DOUBLE, answer_pref, counts, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime() - start_time;
    double total_time;

    MPI_Reduce(&end_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if(rank==0){
      for(int i=0;i<n;i++){
        cout<<answer_pref[i]<<' ';
      }
      cout<<endl;
      // cout<<"Total time taken(s) : "<<total_time<<"\n";
    }

    // Clean up befpre exiting
    MPI_Finalize(); 
    return 0;
}
