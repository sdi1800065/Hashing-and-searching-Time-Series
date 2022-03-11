#include <iostream>
#include "FileHandling.hpp"
#include "functionalities.hpp"
#include <sstream>

ifstream input_file;

ifstream query_file;

ofstream output_file;

map <int, string> ids;
map <int, string> query_ids;

int number_of_lines(char c)
{
  int lines=0;
	string vec;
  if (c == 'i') {                                 //If the file is the input file
    while( getline(input_file,vec) )++lines;     //Count lines
  	input_file.clear();
  	input_file.seekg(0,ios::beg);
  }
  else {                                    //If is the query file
    while( getline(query_file,vec) ) ++lines;   //Count lines
  	query_file.clear();
  	query_file.seekg(0,ios::beg);
  }
 
	return lines;
}

const std::string WHITESPACE = " \n\r\t\f\v";
string vector_data(char c,int ii)
{
    int dim = 0;
    string vec;
    if (c == 'i') getline(input_file,vec);          //If its the input file
    else {
      getline(query_file,vec);                      //Take the line
    }
    std::istringstream iss(vec+"END");
    string word;
    
    iss >>word;
    if (c == 'i') ids.insert({ii,word});          //Map the index with id
    else query_ids.insert({ii,word});
    
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