#pragma once
#include <fstream>


using namespace std;

//The 3 files needed

extern ifstream input_file;

extern ifstream query_file;

extern ofstream output_file;

int number_of_lines(char c);      //Get the number of lines of a file
string vector_data(char c,int i); //Get each query
