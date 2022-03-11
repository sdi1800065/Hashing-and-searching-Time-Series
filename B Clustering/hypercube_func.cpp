#include "hypercube_func.hpp"
#include "functionalities.hpp"
#include "clusterring_functions.hpp"
#include <ctime>
#include <limits>
#include <climits>
#include <cmath>
#include <random>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <iomanip>

unordered_map <int,char> bits;
unordered_map <string, vector <int>> cube;
vector <vector <int> > h_hc;
vector<vector<double>> v_hc;

int n_hc;
int w_hc = 4000;

void create_h_functionsHC()                                                   //Create the needed functions for hypercube
{
  h_hc.resize(k_hyper);
  srand(time(NULL));
  v_hc.resize(n);
  for (int i=0; i<k_hyper; i++) h_hc[i].resize(n);                //Initiallize h
  int dimension = p[0].size();                                    //Take dimension
  for (int i=0; i<p.size(); i++) {                                //For each vector-point
    string bit_string;
    //Compute v
    for (int y = 0; y<k_hyper; y++) {
      v_hc[i].resize(dimension);
      for (int j = 0; j < dimension; j++) {
          double a = normal_distribution_generator();
          v_hc[i][j] = a;
      }
      double innerProduct = inner_product(p[i],v_hc[i], dimension);                        //Compute inner product of a nd v
      float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w_hc));       //Take a random t
      double sum = innerProduct + t;
      //Compute h
      h_hc[y][i] = floor(double(sum/(double) w_hc));
      f(h_hc[y][i]);                    //Take 0 or 1 for h function
      char c = bits[h_hc[y][i]];
      bit_string.push_back(c);        //Create the bit string for the vector-point
    }
    if ( cube.count(bit_string) > 0 ) {             //If there are other vector-points in this probe
        cube[bit_string].push_back(i);              //Push back the vector-point to the vector
    }
    else {                                        //Else
      vector < int > v;                         //Create a new vector with is vector-point
      v.push_back(i);
      cube[bit_string] = v;
    }
    marked.insert({i, false});            //At first all vector-points are all mapped to false
  }
}

void f(int x)
{
    if ( bits.count(x) > 0 ) {            //If we have already mapped x to a bit do nothing

    }
     else {                                   //Else map it
       char c;
       int r = rand()%2;
       if (r == 0) c = '0';
       else c = '1';
       bits.insert({x, c});
    }

}

string hash_queryHC(vector<double> query)                   //Hash the query of for hypercube
{
  int dimension = query.size();
  string bit_string;
  //Create the bit string
  for (int i=0; i<k_hyper; i++) {
    vector<double> ndg;
    ndg.resize(dimension);
    for (int i = 0; i < dimension; i++) {
        double a = normal_distribution_generator();
        ndg[i] = a;
    }
    double innerProduct = inner_product(query,ndg, dimension);
    float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w_hc));
    double sum = innerProduct + t;
    long long unsigned int h = floor(double(sum/(double) (w_hc)));
    f(h);
    char c = bits[h];
    bit_string.push_back(c);
  }
  return bit_string;
}




vector<int> bf_range(int id, vector<double> query,float r)                      //Range search with brute force method
{
  vector<int>b;
  bool f;
  for (int i=0; i<p.size(); i++) {
      double euc_dist = euclidian_distance(query, p[i]);    //Calculate all distances
      if (euc_dist < r){                                    //If it's smaller than our radius we want this vector-point
        b.push_back(i);
      }
  }
  return b;
}


vector <int> range_searchHC(int id, vector<double> query, float r)
{
  vector<int> b;
  vector<double> dist;
  bool f;
  string bucket = hash_queryHC(query);
  int checked_probes = 0,checked_points = 0, bit = 0, h_dist = 1;
  while (checked_probes < probes && checked_points < M) {
    //Same final conditions as in knn
    if (cube.count(bucket) == 0) { //Bucket is not found
      if (bit == nChoosek(bucket.size(),h_dist)) {      //When you find all the possible combiantions hamming distance dist of our bucket
          h_dist++;
          bucket = hamming_distance(h_dist, bit, bucket);
      }
      else {
        bucket = hamming_distance(h_dist, bit, bucket);
      }
    }
    else {
      for (int j=0; j<cube[bucket].size(); j++) {  //For each vector-point in this bucket
        if (exists(b,cube[bucket][j])) continue;  //If we have already checked this vector-point
        checked_points++;
        if (checked_points > M) break;
        double euc_dist = euclidian_distance(query, p[cube[bucket][j]]); //Compute the euclidian distance
        if (euc_dist < r) b.push_back(cube[bucket][j]); //If the distance is smaller than R
      }
      bucket = hamming_distance(h_dist,checked_probes, bucket);
    }
    checked_probes++;
  }
  if (b.size() == 0) b = bf_range(id, query,r);
  return b;
}
