#pragma once
#include <vector>
#include <unordered_map>
#include "functionalities.hpp"


extern unordered_map <int,char> bits;                           //Save 0 or 1 that each integer maps to
extern unordered_map <string, vector <int>> cube;               //Data structure for our Hypercube
extern vector <vector <int> > h_hc;                             //h functions
extern vector<vector<double>> v_hc;                             //g functions
extern int TableSize;


void f(int x);                              //Return 0 or 1
void store_points();                        //Save all vector-points (for Hypercube)
string hash_queryHC(vector<double> query);    //Hash the query and take the probe
void save_vectorsHC();                     //Save all vector-points (for Hypercube)
void create_h_functionsHC();                //Create h functions for hypercube
vector<int> range_searchHC(int id, vector<double> query, float r);  //Range search with hypercube method
