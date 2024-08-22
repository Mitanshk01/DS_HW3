#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
  // Initialize MPI Environment
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Variable Declarations
  int n, m, k;
  pair<double, double> *p;  // Points 
  pair<double, double> *q;  // Queries 
  pair<double, double> *rec_p;  // Received points
  pair<double,pair<double,double>> *sorted_p; // Sorted points for every query in every proc
  pair<double, pair<double, double>> *all_p; // Store best k of all procs in parent proc
  int *counts = (int*)malloc(sizeof(int) * size);
  int *displacements = (int*)malloc(sizeof(int) * size);
  int *counts_gather = (int*)malloc(sizeof(int) * size);
  int *displacements_gather = (int*)malloc(sizeof(int) * size);

  if(rank == 0) {
      cin >> n >> m >> k;
  }

  double start_time = MPI_Wtime();
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

  p = (pair<double, double>*)malloc(sizeof(pair<double, double>) * n);
  q = (pair<double, double>*)malloc(sizeof(pair<double, double>) * m);

  if(rank == 0) {
    for(int i=0;i<n;i++) {
        cin >> p[i].first >> p[i].second;
    }
    for(int i=0;i<m;i++) {
        cin >> q[i].first >> q[i].second;
    }
  }

  MPI_Bcast(q, m * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  int cnt_total = 0;
  for(int i=0;i<size;i++){
    counts[i]=n/size;
    int rem=n%size;
    if(rem>i){
      counts[i]++;
    }
    counts[i]*=2;
    displacements[i]=cnt_total;
    cnt_total+=counts[i];
  }

  // Optimize here?
  rec_p = (pair<double, double>*)malloc(sizeof(pair<double, double>) * k);

  MPI_Scatterv(p, counts, displacements, MPI_DOUBLE, rec_p, counts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < size; i++) {
    counts_gather[i] = k * 3; 
    displacements_gather[i] = i * k * 3;
  }

  for(int i=0;i<m;i++){
    vector<pair<double,pair<double,double>>> dist_p;
    for(int j=0;j<counts[rank]/2;j++){
      pair<double, double>el = rec_p[j];
      double dist=(el.first-q[i].first)*(el.first-q[i].first)+(el.second-q[i].second)*(el.second-q[i].second);
      dist_p.push_back({dist,{el}});
    } 
    sort(dist_p.begin(),dist_p.end());
    sorted_p = (pair<double,pair<double,double>>*)malloc(sizeof(pair<double,pair<double,double>>) * k);
    for(int j=0;j<k;j++){
      if(j>=dist_p.size()){
        sorted_p[j]={1e12,{-1,-1}};
      }else{
        sorted_p[j]=dist_p[j];
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0){
      all_p = (pair<double, pair<double, double>>*)malloc(sizeof(pair<double, pair<double, double>>) * k * size);
    }

    MPI_Gatherv(sorted_p, k * 3, MPI_DOUBLE, all_p, counts_gather, displacements_gather, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(rank == 0){
      int tot_size = k*size;
      sort(all_p, all_p + tot_size);
      for(int j=0;j<k;j++){
        cout<<all_p[j].second.first<<' '<<all_p[j].second.second<<endl;
      }
    }
  }

  double elapsed_time = MPI_Wtime() - start_time;
  double total_time;

  MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if(rank==0){
    cout<<"Total time taken(s) : "<<total_time<<"\n";
  }

  MPI_Finalize(); 
  return 0;
}