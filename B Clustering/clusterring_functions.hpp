#pragma once
#include <vector>
using namespace std;

extern string update_method;
extern string assignment_method;
extern vector <vector <double>> centers;
extern vector <vector <int>> clusters;


void kmeans_plusplus();                           //K means ++ algorithm for centroid initiallization
void Lloyds_assignment();                          //Assign vector-points to each centroid using Lloyds algorithm
void reverse_assignment();                             //Reverse assignment with Lsh approximate algorithm
bool update_centroids();                          //Each time creates the new-better centorids
double min_distance_among_centers();              //Minimum distances between 2 centroids
