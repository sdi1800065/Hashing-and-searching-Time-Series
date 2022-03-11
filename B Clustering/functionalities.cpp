#include <string.h>
#include <cmath>
#include "functionalities.hpp"
#include "clusterring_functions.hpp"
#include <random>
#include <map>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <climits>
#include <algorithm>

int n;
int k, L, N, R;
int K,k_hyper,probes,M;
bool complete;
bool silhouette;
vector<vector<double>> p;
string update_method;
string assignment_method;
map <int,bool> marked;
map <int, string> ids;

ifstream input_file;
ofstream output_file;
ifstream config_file;

using namespace std;

void handle_cluster_args(int argc, char *argv[])                  //Handle argc and argv for clustering
{

  for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i],"-i") == 0) input_file.open(argv[i+1]);
      else if (strcmp(argv[i],"-c") == 0) {
        config_file.open(argv[i+1]);
        inspect_config_file();
      }
      else if (strcmp(argv[i],"-complete") == 0) {
        complete = true;
      }
      else if (strcmp(argv[i],"-silhouette") == 0) {
        silhouette = true;
      }
      else if (strcmp(argv[i],"-update") == 0) update_method = argv[i+1];
      else if (strcmp(argv[i],"-assignment") == 0) assignment_method = argv[i+1];
      else if(strcmp(argv[i],"-ο")==0 || strcmp(argv[i],"-o")==0)
      {
        if(argv[i+1]==nullptr)
        {
          std::cout<<"ERROR Expected name of output_file after -o,file not given or the given file doesnt exist\n";
          exit(-1);
      }
      output_file.open(argv[i+1]);
    }
  }
  if(output_file.is_open() == 0 || input_file.is_open() == 0 || output_file.is_open()==0)
  {
    std::cout<<"ERROR input or output files are missing\n";
    exit(-1);
  }
}


void inspect_config_file()                                              //Take the arguments from the config file
{
  string line;
  while( getline(config_file,line) ) {
      if (line.find("number_of_clusters")!=std::string::npos) {
        string str = "number_of_clusters: ";
        K = stoi(line.substr(str.size(),line.size()));
      }
      else if (line.find("number_of_vector_hash_tables")!=std::string::npos) {
        string str = "number_of_vector_hash_tables: ";
        L = stoi(line.substr(str.size(),line.size()));
      }
      else if (line.find("number_of_vector_hash_functions")!=std::string::npos) {
        string str = "number_of_vector_hash_functions: ";
        k = stoi(line.substr(str.size(),line.size()));
      }
      else if (line.find("max_number_M_hypercube")!=std::string::npos) {
        string str = "max_number_M_hypercube: ";
        M = stoi(line.substr(str.size(),line.size()));
      }
      else if (line.find("number_of_hypercube_dimensions")!=std::string::npos) {
        string str = "number_of_hypercube_dimensions: ";
        k_hyper = stoi(line.substr(str.size(),line.size()));
      }
      else if (line.find("number_of_probes")!=std::string::npos) {
        string str = "number_of_probes: ";
        probes = stoi(line.substr(str.size(),line.size()));
      }
  }
}

void save_curves()                                                           //Store all vector-points
{
  n = number_of_lines('i');
  p.resize(n);
  int dimension;
  string vec;
  for (int i=0; i<p.size(); i++)  {                   //For each vector-point
    vec.assign(vector_data('i',i));
    dimension = vec.size();
    p[i].resize(dimension);
    //This piece of code takes all numbers of the query and put each of them in a cell of array words
    char words[vec.size()][15];
    int j=0, ctr=0;
    for(int l=0;l<=vec.size();l++){
        if(vec[l] == '\t' || vec[l]==' '||vec[l]=='\0'){
            words[ctr][j]='\0';
            ctr++;
            j=0;
        }
        else{
          words[ctr][j]=vec[l];
          j++;
        }
    }
    //Take dimension, crate vector-poimt and save inside p curve
    dimension = ctr;
    p[i].resize(dimension);
    for (j=0; j<dimension; j++) {
      p[i][j] = atof(words[j]);
    }
    marked.insert({i, false});
  }
}

int number_of_lines(char c)
{
  int lines=0;
	string vec;
  if (c == 'i') {
    while( getline(input_file,vec) ) ++lines;       //Take each line and increase the counter
  	input_file.clear();
  	input_file.seekg(0,ios::beg);
  }
	return lines;
}

const std::string WHITESPACE = " \n\r\t\f\v";
string vector_data(char c,int ii)
{
    int dim = 0;
    string vec;
    getline(input_file,vec);          //If its the input file
    std::istringstream iss(vec+"END");
    string word;

    iss >>word;

    if (c == 'i') ids.insert({ii,word});          //Map the index with id
    iss >>word;
    string rest("");
    while(word.compare("END")!=0){
      rest=rest+word+" ";
      iss >>word;
    }
    size_t end = rest.find_last_not_of(WHITESPACE);
    std::string returns=(end == std::string::npos) ? "" : rest.substr(0, end + 1);
    return returns;
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


double normal_distribution_generator()                                  //Take a number from normal distribution
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

double inner_product(vector <double> v1, vector <double> v2, int dimension)           //Compute the inner product
{
    double innerProduct = 0;
    for (int i = 0; i < dimension; i++) {
        innerProduct = innerProduct + v1[i]*v2[i];
    }
    return innerProduct;
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

long long unsigned int modulo(long long unsigned int a, long long unsigned int b)       //Compute the modulo of 2 numbers (supports negatives)
{
  return (a%b + b)%b;
}

bool exists(vector<int> v, int a )
{
  for (int i=0; i<v.size(); i++) {
    if (v[i]==a) return true;                     //If you find the element return true
  }
  return false;
}

int get_index(int a, vector <int> v) {
  for (int i=0; i<v.size(); i++) {
    if (v[i]==a) return i;                          //If you find the element return index
  }
  return -1;
}

int ec = 0;;
double euclidian_distance(vector<double> p1, vector<double> p2)       //Compute the euclidian distance
{
  ec++;
  int dimension = p1.size();
  double dist = 0;
  for (int i=0; i<dimension; i++) {
      dist += (p1[i]-p2[i])*(p1[i]-p2[i]);
  }
  dist = sqrt(dist);
  //cout << ec << endl;
  return dist;
}


double f(vector<double> curve1, vector<double> curve2)
{
  double avg = 0;
  for (int i=0; i<curve1.size(); i++) {
    avg += (abs(curve1[i]-curve2[i])/double(curve1[i]));
  }
  avg /= curve1.size();
  return avg;
}
