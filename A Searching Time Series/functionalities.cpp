#include "functionalities.hpp"
#include "FileHandling.hpp"
#include <string.h>
#include <random>
#include <map>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>     /* abs */


int k,k_hyper, L, N,M,probes,w;
float delta;
std::string algorithm,metric;

inline bool file_exists (const std::string& name) {   //Check if a file exists
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void handle_arguments(int argc, char *argv[])             //Handle argc and argv
{
  k=4;
  L=5;
  k_hyper=14;
  M=10;
  probes=2;
  N=1;
  std::string temp;
  bool deltaflag=0;
  for (int i = 0; i < argc; i++) {
    temp=argv[i];
    std::string temp=argv[i];
    if(temp.compare("–k")==0 || temp.compare("-k")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the number k after -k\n";
        exit(-1);
      }
      std::string temp2=argv[i+1];
      int j;
      for (j=0;j<temp2.length();j++)
      {
        if( temp2[j]<'0' || temp2[j]>'9')
        {
          std::cout<<"ERROR invalid intiger after -k, characters were detected\n";
          exit(-1);
        }
      }
      k=std::stoi(argv[i+1]);
      k_hyper=std::stoi(argv[i+1]);
    }
    else if(temp.compare("-L")==0 || temp.compare("-L")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the number after L -L\n";
        exit(-1);
      }
      std::string temp2=argv[i+1];
      int j;
      for (j=0;j<temp2.length();j++)
      {
        if( temp2[j]<'0' || temp2[j]>'9')
        {
          std::cout<<"ERROR invalid intiger after -L, characters were detected\n";
          exit(-1);
        }
      }
      L=std::stoi(argv[i+1]);
    }
    else if(temp.compare("-M")==0 || temp.compare("-M")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the number  M after -M\n";
        exit(-1);
      }
      std::string temp2=argv[i+1];
      int j;
      for (j=0;j<temp2.length();j++)
      {
        if( temp2[j]<'0' || temp2[j]>'9')
        {
          std::cout<<"ERROR invalid intiger after -M, characters were detected\n";
          exit(-1);
        }
      }
      M=std::stoi(argv[i+1]);
    }
    else if(temp.compare("-probes")==0 || temp.compare("-probes")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the number probes after -probes\n";
        exit(-1);
      }
      std::string temp2=argv[i+1];
      int j;
      for (j=0;j<temp2.length();j++)
      {
        if( temp2[j]<'0' || temp2[j]>'9')
        {
          std::cout<<"ERROR invalid intiger after -probes, characters were detected\n";
          exit(-1);
        }
      }
      probes=std::stoi(argv[i+1]);
    }
    else if(temp.compare("-delta")==0 || temp.compare("-delta")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the number delta after -delta\n";
        exit(-1);
      }
      std::string temp2=argv[i+1];
      delta=std::stof(argv[i+1]);
      deltaflag=1;
    }
    else if(temp.compare("-algorithm")==0 || temp.compare("-algorithm")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the name of the algorithm after -algorithm\n";
        exit(-1);
      }
      algorithm=argv[i+1];
    }
    else if(temp.compare("-metric")==0 || temp.compare("-metric")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR expected the name of the algorithm after -algorithm\n";
        exit(-1);
      }
      metric=argv[i+1];
    }
    else if(temp.compare("–i")==0 || temp.compare("-i")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR Expected name of query_file after -i,file not given or the given file doesnt exist\n";
        exit(-1);
      }
      if(!file_exists(argv[i+1]))
      {
        std::cout<<"ERROR Expected name of input_id after -i ,file not given or the given file doesnt exist\n";
        exit(-1);
      }
      input_file.open(argv[i+1]);
    }
    else if(temp.compare("–q")==0 || temp.compare("-q")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR Expected name of query_file after -q,file not given or the given file doesnt exist\n";
        exit(-1);
      }
      if(!file_exists(argv[i+1]))
      {
        std::cout<<"ERROR Expected name of query_file after -q,file not given or the given file doesnt exist\n";
        exit(-1);
      }
      query_file.open(argv[i+1]);
    }
    else if(temp.compare("-ο")==0 || temp.compare("-o")==0)
    {
      if(argv[i+1]==nullptr)
      {
        std::cout<<"ERROR Expected name of output_file after -o,file not given or the given file doesnt exist\n";
        exit(-1);
      }
      output_file.open(argv[i+1]);
    }
  }

  while(input_file.is_open() == 0)
  {
    std::cout<<"Please give a valid path for the dataset file\n";
    std::string temps;
    std::cin>>temps;
    if(!file_exists(temps))continue;
    input_file.open(temps);
  }
  while(query_file.is_open() == 0)
  {
    std::cout<<"Please give a valid path for the query file\n";
    std::string temps;
    std::cin>>temps;
    if(!file_exists(temps))continue;
    query_file.open(temps);
  }
  while(output_file.is_open() == 0)
  {
    std::cout<<"Please give a valid path for the output file, or a name to make a new one\n";
    std::string temps;
    std::cin>>temps;
    output_file.open(temps);
  }
  while(algorithm.compare("LSH")!=0 && algorithm.compare("Hypercube")!=0 && algorithm.compare("Frechet")!=0){std::cout<<"Give a valid algorithm (LSH | Hypercube | Frechet)\n";std::cin>>algorithm;}

  if(algorithm.compare("Frechet")==0)
  {
    while(metric.compare("discrete")!=0 && metric.compare("continuous")!=0)
    {
      std::cout<<"Give a valid metric (discrete or continuous) \n";
      std::cin>>metric;
    }
    if(deltaflag==0)
    {
      std::cout<<"Error you forgot to give -delta argument,run the program again and please give it\n";
      exit(-1);
    }
    L=1;
  }
  if(algorithm.compare("LSH")==0)
  {
    w=150;
  }else if(algorithm.compare("Hypercube")==0)
  {
    w=4000;
  }else if(algorithm.compare("Frechet")==0 && metric.compare("discrete")==0)
  {
    w=650;
  }else
  {
    w = 1500;
  }
}

double normal_distribution_generator()                                                    //Return a number from normal distribution
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

double inner_product(vector <double> v1, vector <double> v2, int dimension)                                 //Return the inner product of each vector
{
    double innerProduct = 0;
    for (int i = 0; i < dimension; i++) {
      innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
}

long long unsigned int modulo(long long unsigned int a, long long unsigned int b)               //modulo (NOT) remainder, supports negative numbers
{
  return (a%b + b)%b;
}
bool exists(vector<int> v, int a )                          //Check if a exists in v
{
  for (int i=0; i<v.size(); i++) {
    if (v[i]==a) return true;
  }
  return false;
}

double euclidian_distance(vector<double> p1, vector<double> p2)       //Compute euclidian distance
{
  int dimension = p1.size();
  double dist = 0;
  for (int i=0; i<dimension; i++) {
      dist += (p1[i]-p2[i])*(p1[i]-p2[i]);
  }
  dist = sqrt(dist);
  return dist;
}

string hamming_distance(int dist, int i, string bit_string) //compute hamming_distance
{
  string binary = bit_string;
  if (dist == 1) {                                      //If dist == 1 just change one bit
    if (binary[i] == '1') binary[i] = '0';
    else binary[i] = '1';
  }
  else {                                                  //Else
    vector <int> bit_index;
    for(int i=0; i<bit_string.size(); i++) bit_index.push_back(i);                //Push_back all indexes
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(bit_index.begin(), bit_index.end(), std::default_random_engine(seed)); //shuffle them
    for (int j = 0; j < dist; j++) {                                              //Change randomly dist bits
      if (binary[bit_index[j]] == '1') binary[bit_index[j]] = '0';
      else binary[bit_index[j]] = '1';
    }
  }
  return binary;
}

unsigned nChoosek( unsigned n, unsigned k )                   //n ana k
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}


double Frechet_distance(vector<double> p1, vector<double> p2)       //Compute euclidian distance
{
  int m1=p1.size(), m2=p2.size(),i,j;
	double distance;
	vector<vector<double>>array;
  double min1;
  double min2;
  array.resize(m1);

  for (i = 0 ; i < m1 ; i++)
	{
    	array[i].resize(m2);
	}
  array[0][0] =abs(p1[0]-p2[0]);
  for(j = 1 ; j < m2 ; j++){
    array[0][j] = max(array[0][j-1],abs(p1[0]-p2[j]));
  }
  for(i = 1 ; i < m1 ; i++){
    array[i][0] = max(array[i-1][0],abs(p1[i]-p2[0]));
  }

  for (i = 1 ; i < m1 ; i++){
  	for (j = 1 ; j < m2 ; j++){
      min1=min(array[i-1][j], array[i-1][j-1]);
      min2=min(min1,array[i][j-1]);
      array[i][j] = max(min2,abs(p1[i]-p2[j]));
    }
  }

	distance = array[m1-1][m2-1];
	return distance;
}
