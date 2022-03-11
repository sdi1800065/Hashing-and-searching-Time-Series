#include "hash.hpp"
#include "functionalities.hpp"
#include "FileHandling.hpp"
#include <ctime>
#include <climits>
#include <cmath>
#include <random>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

//unordered is better for lookup ,we dont need to sort
unordered_map <int,char> bits;                      //Holds 0/1 values of the buckets
unordered_map <string, vector <int>> cube;          //Hypercubes master structure
vector <vector <int> > h_hc;                           //Holds h numbers
vector<vector<double>> p;                           //Holds points
vector<vector<double>> v;

int n;
//After many tests 3500 seems to have the best results

//Stores the points.
void save_vectors()
{
  n = number_of_lines('i');
  p.resize(n);
  int dimension;

  string vec;
  for (int i=0; i<p.size(); i++)  {

    vec.assign(vector_data('i',i));

    dimension = vec.size();
    p[i].resize(dimension);
    for (int y = 0; y < k_hyper; y++) {
        //This piece of code takes all numbers of the vector-point and put eache of them in a cell of array words
        char words[vec.size()][15];
        int j=0, ctr=0;
        for(int l=0;l<=vec.size();l++){
            if(vec[l]==' '||vec[l]=='\0'){
              words[ctr][j]='\0';
              ctr++;
              j=0;
            }
            else{
              words[ctr][j]=vec[l];
              j++;
            }
        }
        dimension = ctr;
        p[i].resize(dimension);
        for (j=0; j<dimension; j++) {
          p[i][j] = atof(words[j]);
        }
    }
  }

}

//Creates h functions and also maps buckets to {0,1} uniformly.
void create_h_functions()
{
  h_hc.resize(k_hyper);
  srand(time(NULL));
  v.resize(n);

  for (int i=0; i<k_hyper; i++) h_hc[i].resize(n);
  int dimension = p[0].size();
  for (int i=0; i<p.size(); i++) {       //For each vector-point
    string bit_string;
    for (int y = 0; y<k_hyper; y++) {           //k times
      v[i].resize(dimension);
      //Create the v vector

      for (int j = 0; j < dimension; j++) {
          double a = normal_distribution_generator();
          v[i][j] = a;
      }

       //Compute the h function
      double innerProduct = inner_product(p[i],v[i], dimension);
      float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w));
      double sum = innerProduct + t;
      h_hc[y][i] = floor(double(sum/(double) w));

      //Coinflip
      f(h_hc[y][i]);
      char c = bits[h_hc[y][i]];
      //Gather coinflips to get the bucket key
      bit_string.push_back(c);
    }
    //if a bucket for this key has already been created push back the id , else make a new bucket and push back the id
    if ( cube.count(bit_string) > 0 ) {
        cube[bit_string].push_back(i);
    }
    else {
      vector < int > v;
      v.push_back(i);
      cube[bit_string] = v;
    }
  }
}

//Randomly choose 1 or 0 to map h(p) , if already choosen for this x then do nothing.
void f(int x)
{
  if ( bits.count(x) > 0 ) {

  }
    else {
      char c;
      int r = rand()%2;
      if (r == 0) c = '0';
      else c = '1';
      bits.insert({x, c});
  }

}

//Returns the bucket that the closest neighboughrs of a query should be
string hash_query_HC(vector<double> query)
{
  int dimension = query.size();
  string bit_string;
  for (int i=0; i<k_hyper; i++) {
    vector<double> ndg;
    ndg.resize(dimension);
    //Create the v vector
    for (int i = 0; i < dimension; i++) {
        double a = normal_distribution_generator();
        ndg[i] = a;
    }
    //Calculate h
    double innerProduct = inner_product(query,ndg, dimension);
    float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w));
    double sum = innerProduct + t;
    long long unsigned int h = floor(double(sum/(double) (w)));
    //Coinflip
    f(h);
    char c = bits[h];
    //Gather coinflips to get the bucket key
    bit_string.push_back(c);
  }
  return bit_string;
}


vector<vector<vector<long long unsigned int>>> hash_tables;
vector<vector<vector<double>>> h;
vector<vector<int>> g;
vector<int> r;
vector <vector <long long unsigned int>> IDS;
vector<vector<std::pair<int,double>>> Grid2D;      //just for storing , in practice id does nothing
vector<vector<double>> p_L;                        //snaped and pad curves
float t1_L;
int TableSize;

void create_hash_tables(std::string algorithm)
{
  srand(time(NULL));
  n = number_of_lines('i');
  TableSize = ceil(n/8.0);
  hash_tables.resize(L);                              //L hash tables
  for (int i=0; i<L; i++) hash_tables[i].resize(TableSize);   //Each has size == n
  h.resize(L);                                        //L "teams" of hash functions
  for (int i=0; i<L; i++) h[i].resize(k);             //Each team has k hash functions
  for (int i=0; i<L; i++) {                           //Each hash function is for one vector-point
    for (int j=0; j<k; j++) h[i][j].resize(n);
  }
  g.resize(L);                                      //L "teams" of g functions
  for (int i=0; i<L; i++) g[i].resize(n);           //Each g is for one vector-point
  r.resize(k);                                  //r vector
  p.resize(n);

  if(algorithm.compare("Frechet")==0 && metric.compare("discrete")==0){
    Grid2D.resize(n);
    p_L.resize(n);
    int dimension;
    string vec;
    for (int i=0; i<p.size(); i++)  {
      vec.assign(vector_data('i',i));                   //Take the vector
      dimension = vec.size();
      Grid2D[i].resize(dimension);

      //This piece of code takes all numbers of the vector-point and put eache of them in a cell of array words
      char words[vec.size()][15];
      int j=0, ctr=0;
      for(int l=0;l<=vec.size();l++){
        if(vec[l]==' '||vec[l]=='\0'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
        }
        else{
          words[ctr][j]=vec[l];
          j++;
        }
      }

      dimension=ctr;
      Grid2D[i].resize(dimension);
      for (j=0; j<dimension; j++) {
        Grid2D[i][j]=std::make_pair(j+1,atof(words[j]));
        p[i].push_back(j+1);
        p[i].push_back(atof(words[j]));
      }
    }
  }
  else
  {
    int dimension;
    string vec;
    for (int i=0; i<p.size(); i++)  {
      vec.assign(vector_data('i',i));
      dimension = vec.size();
      p[i].resize(dimension);

      //This piece of code takes all numbers of the vector-point and put eache of them in a cell of array words
      char words[vec.size()][15];
      int j=0, ctr=0;
      for(int l=0;l<=vec.size();l++){
        if(vec[l]==' '||vec[l]=='\0'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
        }
        else{
          words[ctr][j]=vec[l];
          j++;
        }
      }
      dimension = ctr;
      p[i].resize(dimension);
      for (j=0; j<dimension; j++) {
        p[i][j] = atof(words[j]);     //Create the vector-point
      }
    }

  }
  IDS.resize(L);
  for (int i=0; i<L; i++) IDS[i].resize(n);
  v.resize(n);
}


void create_hash_functions(int table,std::string algorithm)
{
  for (int j = 0; j < k && table==0; j++) {               //Initiallize the random r vector (only one time)
    r[j] = rand()%(INT_MAX);
  }
  if(algorithm.compare("LSH")==0 && metric.compare("discrete")==0){
    int dimension;

    string vec;
    float float1,float2;
    t1_L=static_cast <float> (rand()) / static_cast <float> (RAND_MAX/delta);

    for (int i=0; i<Grid2D.size(); i++)  {                       //For each vector-point

      //Snap and pad
      int jj=0;
      bool flagd=0;
      dimension = Grid2D[i].size();
      for(int j=0;j<dimension;j++)
      {
        float1=t1_L+floor(Grid2D[i][j].first/delta+0.5)*delta;
        float2=t1_L+floor(Grid2D[i][j].second/delta+0.5)*delta;
        for(int jc=0;jc<j;jc++)
        {
          if(p_L[i][2*jc]==float1&&p_L[i][2*jc+1]==float2)
          {
            flagd=1;
            jj+=1;
          }
        }
        if(flagd){flagd=0;continue;}

        p_L[i].push_back(float1);
        p_L[i].push_back(float2);
      }

      for(int j=0;j<jj;j++)
      {
        p_L[i].push_back(9999);
        p_L[i].push_back(9999);
      }

      dimension=p_L[i].size();

      for (int y = 0; y < k; y++) {

        v[i].resize(dimension);
        //Create the v vector
        for (int j = 0; j < dimension; j++) {
          double a = normal_distribution_generator();
          v[i][j] = a;
        }

        //Compute the h function
        float t;
        double innerProduct = inner_product(p_L[i],v[i], dimension);

        t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w));
        double sum = innerProduct + t;
        h[table][y][i] = floor(double(sum/(double) w));
      }

      //Compute g (the bucket)
      long long unsigned int sum = 0;
      long long int number = 0;
      long long unsigned int M1 = 4294967291;
      //Calculate modulo partially to avoid overflows
      for (int j = 0; j < k; j++) {
        number = modulo(r[j]*h[table][j][i],M1);
        sum += number;
      }
      sum = modulo(sum,M1);
      IDS[table][i] = sum;
      sum = modulo(sum,TableSize);
      g[table][i] = sum;
      hash_tables[table][sum].push_back(i);     //Insert the vector-point to the right bucket of the right vector of hash table
    }
  }
  else{
    int dimension;
    string vec;

    for (int i=0; i<p.size(); i++)  {                       //For each vector-point

      dimension = p[i].size();

      for (int y = 0; y < k; y++) {                 //k times

        v[i].resize(dimension);
        //Create the v vector
        for (int j = 0; j < dimension; j++) {
          double a = normal_distribution_generator();
          v[i][j] = a;
        }
        //Compute the h function
        double innerProduct = inner_product(p[i],v[i], dimension);
        float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w));
        double sum = innerProduct + t;
        h[table][y][i] = floor(double(sum/(double) w));
      }
      //Compute g (the bucket)
      long long unsigned int sum = 0;
      long long int number = 0;
      long long unsigned int M1 = 4294967291;
      //Calculate modulo partially to avoid overflows
      for (int j = 0; j < k; j++) {
        number = modulo(r[j]*h[table][j][i],M1);
        sum += number;
      }
      sum = modulo(sum,M1);
      IDS[table][i] = sum;
      sum = modulo(sum,TableSize);
      g[table][i] = sum;
      hash_tables[table][sum].push_back(i);     //Insert the vector-point to the right bucket of the right vector of hash table
    }
  }
}
vector<double>frechet_querry(vector<double> query){
  int dimension = query.size();
  int jj=0;
  bool flagd=0;
  float float1,float2;
  vector<double>hashvector;
  for(int j=0;j<dimension/2;j++)
  {
    float1=t1_L+floor(query[2*j]/delta+0.5)*delta;
    float2=t1_L+floor(query[2*j+1]/delta+0.5)*delta;
    for(int jc=0;jc<hashvector.size();jc++)
    {
      if(hashvector[jc]==float1&&hashvector[jc+1]==float2)
      {
        flagd=1;
        jj+=1;
      }
    }
    if(flagd){flagd=0;continue;}
    hashvector.push_back(float1);
    hashvector.push_back(float2);
  }
  for(int j=0;j<jj;j++)
  {
    hashvector.push_back(9999);
    hashvector.push_back(9999);
  }
  return hashvector;
}

long long int hash_query(vector<double> query, long long unsigned int *ID)
{
  if(algorithm.compare("Frechet")==0 && metric.compare("discrete")==0)
    query=frechet_querry(query);
  int dimension = query.size();
  vector < long long unsigned int> h_function;
  h_function.resize(k);
  //Calculate each of k h functions
  for (int y = 0; y < k; y++) {
    vector<double> ndg;
    ndg.resize(dimension);
    //Create the v vector
    for (int i = 0; i < dimension; i++) {
        double a = normal_distribution_generator();
        ndg[i] = a;
    }
    double innerProduct = inner_product(query,ndg, dimension);
    float t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/w));
    double sum = innerProduct + t;
    h_function[y] = floor(double(sum/(double) w));
  }
  //Calculate g function
  long long unsigned int g = 0;
  long long int number = 0;
  long long unsigned int M = 4294967291;
  for (int j = 0; j < k; j++) {
      number = modulo(r[j]*h_function[j],M);
      g += number;
  }
  g = modulo(g,M);
  *ID = g;
  g = modulo(g,TableSize);

  return g;
}


void store_to_hash_table(vector <double> curve, int id)
{
  long long unsigned int ID;
  int bucket = hash_query(curve, &ID);
  hash_tables[0][bucket].push_back(id);
}
