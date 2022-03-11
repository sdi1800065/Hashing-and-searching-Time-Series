#pragma once
#include <vector>
#include <unordered_map>
#include "FileHandling.hpp"


extern unordered_map <int,char> bits;               //Holds 0/1 values of the buckets
extern unordered_map <string, vector <int>> cube;   //Hypercubes master structure
extern int n;  
extern vector <vector <int> > h_hc;                    //Holds h numbers

extern vector<vector<vector<long long unsigned int>>> hash_tables;          //All hash tables

extern vector<vector<double>> v;                                            //v vectors


extern vector<vector<int>> g;                                               //g functions

extern vector<int> r;                                                        //r vector


extern vector<vector<vector<double>>> h;                                     //h functions

extern vector<vector<double>> p;                                             //All points-vectors from input_file

extern vector <vector <long long unsigned int>> IDS;                        //The IDS for the querying trick

extern int TableSize;                                                       //The size of eaxh hash tale

extern vector<vector<std::pair<int,double>>> Grid2D;                        //Just for storing ,in practice it does nothing



void create_hash_tables(std::string);                                                  //Create all hash_tables and initiallize all required vectors
void create_hash_functions(int table,std::string);                                      //Create h's ang g's functions
vector<double>frechet_querry(vector<double> query);
long long int hash_query(vector<double> query,long long unsigned int *ID);  //Hash the query and take the bucket

void f(int x);                                  //Randomly choose 1 or 0 to map h(p) , if already choosen for this x then do nothing.
string hash_query_HC(vector<double> query);        //Returns the bucket that the closest neighboughrs of a query should be
void save_vectors();                            //Stores the points.
void create_h_functions();                      //Creates h functions and also maps buckets to {0,1} uniformly.

void store_to_hash_table(vector <double> curve, int id);