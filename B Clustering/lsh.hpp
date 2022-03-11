#pragma once
#include <vector>
#include <map>
using namespace std;

extern int TableSize;                         //Size of hash table



void create_hash_tables();                                  //Create all hash_tables and initiallize all required vectors
void create_hash_functions(int table);                                      //Create h's ang d's functions
vector<int> range_search(int id, vector<double> query, float r);      //Range search with radius r
long long int hash_query(vector<double> query);                     //Hash the query and take the bucket
double _distance(vector<double> p1, vector<double> p2);
