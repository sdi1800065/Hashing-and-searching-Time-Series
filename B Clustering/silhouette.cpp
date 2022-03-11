#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include "functionalities.hpp"
#include "silhouette.hpp"
#include "lsh.hpp"
#include "clusterring_functions.hpp"
using namespace std;

vector <double> a;
vector<double> b;
vector<double> s;

void compute_silhouette()
{
  output_file<<"Silhouette: [";
  int j,ll;
  double silhouette = 0;
  double siltotal=0;
  s.resize(clusters.size());
  for (int i=0; i<clusters.size(); i++){
    silhouette = 0;
    //Initiallize a,b and c
    a.resize(clusters[i].size());
    s.resize(clusters[i].size());
    b.resize(clusters[i].size());
    double sum = 0;
    vector <vector <double>> distance;          //This vector will have all the distances among all vectors
    distance.resize(clusters[i].size());
    for (j=0; j<clusters[i].size(); j++){
      distance[j].resize(clusters[i].size());
      for (int jj=0; jj<clusters[i].size(); jj++) distance[j][jj] = -1;         //Initiallize with -1 which means that the distace from j to jj has not been computed
    }

    for (j=0; j<clusters[i].size(); j++) {
      sum = 0.0;
      for (ll=0; ll<clusters[i].size(); ll++) {
         if (j == ll) distance[j][ll] = 0;                                      //The distance between one vector-point to the same vector-point is obviously 0
         if (distance[j][ll] == -1) {                                            //If the distance has not been computed
           distance[j][ll] = _distance(p[clusters[i][j]], p[clusters[i][ll]]);               //Compute
           distance[ll][j] = distance[j][ll];                                     //The distance from j to ll equals to the distance from ll to j
           sum+=distance[j][ll];                                                  //Add it
         }
         else {                                                                   //Else we have already calculated this distance
            sum+=distance[j][ll];
         }
      }

      a[j] = sum/(double)(clusters[i].size());                                            //Compute a
      int second_best_center = find_second_closest_centroid(clusters[i][j],i);          //Find the centroid that is the second best for this vector-point
       sum = 0.0;
       //Compute b
       for (ll=0; ll<clusters[second_best_center].size(); ll++) {
            double bd = _distance(p[clusters[second_best_center][ll]], p[clusters[i][j]]);
            sum+=bd;
        }
       b[j] = sum/(double)clusters[second_best_center].size();
       //Compute s
       if (a[j] < b[j]) s[j] = 1 - a[j]/b[j];
       else if (a[j] > b[j]) s[j] = b[j]/a[j] - 1;
       else s[j] = 0;
      silhouette+=s[j];
      siltotal+=s[j];
    }
    //Compute silhouette for each cluster
     silhouette /= clusters[i].size();

     output_file << silhouette << ", ";
  }
  output_file << siltotal/(double)p.size()<<"]\n\n";
}

int find_second_closest_centroid(int point, int current_cluster)
{
  double min_dist = numeric_limits<float>::infinity();
  int min_center;
  for (int i=0; i<centers.size(); i++) {
    if (i == current_cluster) continue;                         //Don't take in mind the first best cluster for our vector-point
    double euc_dist = euclidian_distance(p[point], centers[i]);
    //Find minimum
    if (euc_dist < min_dist) {
      min_dist = euc_dist;
      min_center = i;
    }
  }
  return min_center;
}
