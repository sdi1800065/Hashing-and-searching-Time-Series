#include <iostream>
#include <limits>
#include <cmath>
#include "search.hpp"
#include "functionalities.hpp"
#include "FileHandling.hpp"
#include "hash.hpp"
#include "curves.hpp"
#include <algorithm>

tuple_of_vectors knn_LSH(vector<double> query)
{
  tuple_of_vectors tupl;
  tupl.v1.resize(1);
  tupl.v2.resize(1);
  long long unsigned int ID;
  int j;
  tupl.v2[0] = numeric_limits<float>::infinity();                 //Initiallize with infinity the distance table
  tupl.v1[0] = 99999999;                                          //Initiallize with infinity the id table
  int items_found = 0;
  bool redo = false;
  double euc_dist ;
  for (int i=0; i<L; i++) {                        
    int bucket = hash_query(query, &ID);                                   //Hash the query and take each id

    for (j=0; j<hash_tables[i][bucket].size(); j++) {                   //For each vector-point in this bucket
      if (exists(tupl.v1,hash_tables[i][bucket][j])) continue;       //If we have already checked this vector-point
      if (ID != IDS[i][hash_tables[i][bucket][j]] && !redo) {        //Perfrom the querying trick
        continue;
      }
      else {
        items_found++;
      }
      if(algorithm.compare("Frechet")==0){euc_dist = Frechet_distance(query, p[hash_tables[i][bucket][j]]);}
      else euc_dist = euclidian_distance(query, p[hash_tables[i][bucket][j]]);

      if (euc_dist < tupl.v2[0]){
        tupl.v2[0] = euc_dist;
        tupl.v1[0] = hash_tables[i][bucket][j];
      }

    }
    if (i == L - 1) {                         //If we have checked all hash tables
      if (items_found < 1) {                  //If we did not find the number of neighbors that we wanted
        redo = true;                          //Redo the loop without the quering trick
        i = -1;
      }
    }
  }
  if(tupl.v2[0]==numeric_limits<float>::infinity())tupl=bf(query); //in case nothing was found
  //std::cout<<"LSH :"<<tupl.v1[0]<<" "<<tupl.v2[0]<<"\n";
  return tupl;
}

tuple_of_vectors knn_Hypercube(vector<double> query)
{
  tuple_of_vectors tupl;
  tupl.v1.resize(1);
  tupl.v2.resize(1);
  double euc_dist;
  int checked_probes = 0,checked_points = 0;
  int dist = 1;
  int bit = 0;
  tupl.v2[0] = numeric_limits<float>::infinity();                //Initiallize with infinity the distance table
  //tupl.v1[0]=99999999;
  string bucket = hash_query_HC(query);                        //Hash the query and take each id
  while (checked_probes < probes && checked_points < M) { //for each probe
    if (cube.count(bucket) == 0) { //Bucket is not found
      if (bit == nChoosek(bucket.size(),dist)) {      //When you find all the possible combiantions hamming distance dist of our bucket
          dist++;
          bucket = hamming_distance(dist, bit, bucket);
      }
      else {
        bucket = hamming_distance(dist, bit, bucket);
      }
    }
    else {
      for (int j=0; j<cube[bucket].size(); j++) {
          if (checked_points > M) break;                              //If we have checked the maximum number of points stop the loop
          if (exists(tupl.v1,cube[bucket][j])) continue;              //If we have already checked this point continuw with the next one
          checked_points++;
          euc_dist = euclidian_distance(query, p[cube[bucket][j]]);
          //Insert the point while keeping the vector sorted
          if (euc_dist < tupl.v2[0]){
                tupl.v2[0] = euc_dist;
                tupl.v1[0] = cube[bucket][j];
          }
      }
      bucket = hamming_distance(dist,bit, bucket); //get a new bucket to search
    }
    checked_probes++; //increase probes
    bit++;
  }
  if(tupl.v2[0]==numeric_limits<float>::infinity())tupl=bf(query);     //in case nothing was found
  return tupl;
}

tuple_of_vectors bf(vector<double> query)
{
  tuple_of_vectors tupl;
  tupl.v1.resize(1);
  tupl.v2.resize(1);
  double dist;
  tupl.v2[0] = numeric_limits<float>::infinity();               //Initiallize the N-size vector with infinity
  if(algorithm.compare("Frechet")==0 && metric.compare("discrete")==0)
  for (int i=0; i<p.size(); i++) {                                                  //For each point vector
      dist= Frechet_distance(query,p[i]);                            //Compute the euclidian distance
      if (dist < tupl.v2[0]){
        tupl.v2[0] = dist;
        tupl.v1[0] = i;
      }
  }
  else if(algorithm.compare("Frechet")==0)
  for (int i=0; i<p.size(); i++) {                                                  //For each point vector
    dist= distance(query, p[i]);                            //Compute the euclidian distance
    if (dist < tupl.v2[0]){
      tupl.v2[0] = dist;
      tupl.v1[0] = i;
    }
  }
  else
  for (int i=0; i<p.size(); i++) {                                                  //For each point vector
      dist= euclidian_distance(query, p[i]);                            //Compute the euclidian distance
      if (dist < tupl.v2[0]){
        tupl.v2[0] = dist;
        tupl.v1[0] = i;
      }
  }
  return tupl;
}
tuple_of_vectors knn_cont(vector<double> query)
{
  tuple_of_vectors tupl;
  tupl.v1.resize(N);
  tupl.v2.resize(N);
  bool f;
  long long unsigned int ID;
  int j;
  vector <double> key_query = query;
  float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/delta));
  key_query = filter(query);
  key_query = snap(t,query);
  key_query = padding(query,max_dimension);
  for (int i=0; i<N; i++) tupl.v2[i] = numeric_limits<float>::infinity();                 //Initiallize with infinity the distance table
  int items_found = 0;
  bool redo = false;
  for (int i=0; i<L; i++) {                                               //For each hash table
   int bucket = hash_query(query, &ID);                                   //Hash the query and take each id
      for (j=0; j<hash_tables[i][bucket].size(); j++) {                   //For each vector-point in this bucket
            if (exists(tupl.v1,hash_tables[i][bucket][j])) continue;       //If we have already checked this vector-point
            if (ID != IDS[i][hash_tables[i][bucket][j]] && !redo) {        //Perfrom the querying trick
              continue;
            }
            else {
              items_found++;
            }
          double dparray[max_dimension][max_dimension];

          dparray[0][0] = abs(p[hash_tables[i][bucket][j]][0] - query[0]);
           for (int ii = 0; ii < max_dimension; ii++) {
               for (int jj = 0; jj < max_dimension; jj++) {
                   if (ii == 0 && jj > 0) dparray[ii][jj] = max(dparray[ii][jj-1],abs(p[hash_tables[i][bucket][j]][0] - query[jj]));
                    else if (ii>0 && jj==0) dparray[ii][jj] = max(dparray[ii-1][jj], abs(p[hash_tables[i][bucket][j]][ii] - query[0]));
                    else if (ii>0 && jj>0) {
                      double min = dparray[ii-1][jj];
                     if (dparray[ii-1][jj-1] < min) min = dparray[ii-1][jj-1];
                     if (dparray[ii][jj-1] < min) min = dparray[ii][jj-1];
                     dparray[ii][jj] = max(min, abs(p[hash_tables[i][bucket][j]][ii] - query[jj]));
                    }
               }
           }


           double dist = dparray[max_dimension-1][max_dimension-1];
           //double dist = distance(query, p[hash_tables[i][bucket][j]]);
           f = false;
          // //Check if blongs to top N and if so inserted in the vector while keeping it sorted
           if (dist < tupl.v2[N-1]){

             for (int l=0; l<N; l++) {
               if (dist < tupl.v2[l]) {
                 int ll;
                 f = true;
                 for (ll=N-1; ll>l; ll--) {
                   tupl.v2[ll] = tupl.v2[ll-1];
                   tupl.v1[ll] = tupl.v1[ll-1];
                 }
                 tupl.v2[ll] = dist;
                 tupl.v1[ll] = hash_tables[i][bucket][j];
               }
               if (f) break;
             }
           }

      }


      if (i == L - 1) {                         //If we have checked all hash tables
        if (items_found < N) {                  //If we did not find the number of neighbors that we wanted
          redo = true;                          //Redo the loop without the quering trick
          i = -1;
        }
      }
  }
  if(tupl.v2[0]==numeric_limits<float>::infinity())tupl=bf(query); //in case nothing was found
  return tupl;
}