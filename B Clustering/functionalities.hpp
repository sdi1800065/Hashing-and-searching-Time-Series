#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;


//Parameters of the program

extern int n;
extern int k, L, N, R;
extern int K,k_hyper,probes,M;
extern map <int,bool> marked;           //map that assignes points to true or false depending on whether are assigned or not
extern bool complete;
extern bool silhouette;
extern vector<vector<double>> p;
extern ifstream input_file;
extern ofstream output_file;
extern ifstream config_file;

extern map <int, string> ids;


void handle_cluster_args(int argc, char *argv[]);         //Handle the program arguments
unsigned nChoosek( unsigned n, unsigned k );              // n ana k
void inspect_config_file();                               //Take the arguments from the config file
void save_curves();                                      //Save all curves
string hamming_distance(int dist, int i, string bit_string);
int get_index(int a, vector <int> v);                     //Get the index of item a in v
double euclidian_distance(vector<double> p1, vector<double> p2);    //Compute euclidian distance
int number_of_lines(char c);                                    //Take number of lines of a file
string vector_data(char c, int ii);                             //Take each vector from the files
double normal_distribution_generator();                          //Generate a number from normal distribution
double inner_product(vector <double> v1, vector <double> v2, int dimension);    //Compute inner products of 2 vectors
long long unsigned int modulo(long long unsigned int a, long long unsigned int b);  //Compute the modulo (NOT remainder) of 2 numbers (negatives are supported)
bool exists(vector<int> v, int a );                                   //Check if a exists in v
double f(vector<double> curve1, vector<double> curve2);
