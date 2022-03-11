using namespace std;
#include <sstream>
#include "functionalities.hpp"
#include "clusterring_functions.hpp"
#include "silhouette.hpp"
#include "hypercube_func.hpp"
#include "lsh.hpp"
#include <chrono>
#include <ctime>


int main(int argc, char *argv[])
{
  handle_cluster_args(argc, argv);                                         //Handle argc and argv
  std::cout<<"Initializing Clustering\n";
  save_curves();                                                         //Store all vector-points
  std::cout<<"Calling kmeans_plusplus\n";
  auto start1 = std::chrono::system_clock::now();
  kmeans_plusplus();                                                       //Initiallize the centroiids
  if(assignment_method == "Classic") output_file << "Algorithm: Lloyds \n";              //Classic method
  else output_file << "Alogorithm: Range Search "<<assignment_method<<"\n";              //Reverse assignment method
  //
  bool flag = true;
  //
  std::cout<<"Initializing Search";
  if (assignment_method == "LSH_Frechet" || assignment_method == "LSH" ) {                                                  //Initiallize LSH methods
     create_hash_tables();
     for (int i=0; i<L; i++) create_hash_functions(i);
   }
   else if (assignment_method == "Hypercube") create_h_functionsHC();                 //Initiallize Hypercube methods
  //
   std::cout<<"Searching\n";
   int jk = 0;
   //while(jk!=1){
   while(flag){
      bool from_start = false;
       //cout << "while" << endl;
       if (assignment_method == "Classic") Lloyds_assignment();                                          //Assign using classic method
       else if (assignment_method == "LSH_Frechet" || assignment_method == "LSH" || assignment_method == "Hypercube") reverse_assignment();    //Assign using reverse assignment method
       if (from_start == false) flag = !update_centroids();
       jk++;
   }
   auto end1 = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds1 = end1-start1;

   for (int i=0; i<centers.size(); i++) {                                        //For each center
     output_file<<"CLUSTER-"<<i<<" {size :"<<clusters[i].size()<<" ,cendroid: ";
     for (int j=0; j<centers[i].size(); j++){
       if (j!=centers[i].size()-1) output_file << centers[i][j] << ", ";
       else output_file << centers[i][j] << "}";

     }
     output_file<< "\n\n";
   }

   output_file << "clustering_time: " << elapsed_seconds1.count() << "s\n";
   //cout << "clustering_time: " << elapsed_seconds1.count() << "s\n";
  if (silhouette) compute_silhouette();                                                           //Calculate silhouette
  //
   if(complete)
   for (int i=0; i<clusters.size(); i++) {
     output_file<<"CLUSTER-"<<i<<" {size :"<<clusters[i].size()<<" ,cendroid: ";
     for (int j=0; j<centers[i].size(); j++){
       if (j!=centers[i].size()-1) output_file << centers[i][j] << ", ";
       else output_file << centers[i][j] << "}";

     }
     output_file<< "\n\n";
     output_file << "{";
     for (int j=0; j<clusters[i].size(); j++){
       if (j!=clusters[i].size()-1) output_file << ids[clusters[i][j]] << ", ";
       else output_file << ids[clusters[i][j]] << "}";
     }
     output_file<< endl << endl;
   }
}
