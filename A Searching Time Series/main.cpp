using namespace std;
#include <sstream>
#include <chrono>
#include "functionalities.hpp"
#include "FileHandling.hpp"
#include "hash.hpp"
#include "search.hpp"
#include "curves.hpp"

inline bool file_exists (const std::string& name) {   //Check if a file exists
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

int main(int argc, char *argv[])
{
  int flag=0;
  float sec1 = 0, sec2= 0;
  handle_arguments(argc, argv);
  if(algorithm.compare("LSH")==0 || (algorithm.compare("Frechet")==0 && metric.compare("discrete")==0)){

    std::cout<<"Initializing and storing everything ,this might take a while \n";
    create_hash_tables(algorithm);                                         //Create all needed hash tables

    for (int i = 0; i < L; i++) create_hash_functions(i,algorithm);             //Create  all needed hash functions
    std::cout<<"Everyting stored \n";

    while(1){
      if(flag)
      {
        string ans;
        std::cout<<"Do you want to search with a new query_file ( query_file_name / N )\n";
        cin>>ans;
        if(ans.compare("N")==0 || ans.compare("n")==0)break;
        query_file.close();
        if(!file_exists(ans))
        {
          std::cout<<"ERROR Expected name of query_file , given file doesnt exist to terminate type N or n\n";
          continue;
        }
        query_file.open(ans);
      }
      flag=1;
      float sum1 = 0;
      float sum2 = 0;
      float sum = 0;
      std::cout<<"Searching \n";
      long unsigned int number_of_queries = number_of_lines('q');       //Take the number of total queries
      for (int i=0; i<number_of_queries; i++) {                          //For each query
        vector <double> query;
        string vec;
        vec.assign(vector_data('q',i));                                 //Take the query
        char words[vec.size()][15];
        int j=0, ctr=0;
        //This piece of code takes all numbers of the query and put eache of them in a cell of array words

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
        int dimension = ctr;
        //Create the query vector-point as a vector
        for (j=0; j<dimension; j++) { //dont forget to change 120 to dimension
        if(algorithm.compare("Frechet")==0)
          query.push_back(j+1);
          query.push_back( atof(words[j]) );
        }
        //Start the process of the output
        output_file << "Query: "<<query_ids[i]<<"\n";
        if(algorithm.compare("LSH")==0)
          output_file <<"Algorithm: "<<algorithm<<"_Vector\n";
        else
          output_file <<"Algorithm: LSH_"<<algorithm<<"_"<<metric<<"\n";
        auto start1 = std::chrono::system_clock::now();
        tuple_of_vectors a = knn_LSH(query);            //K approximate nearest neighbors
        auto end1 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds1 = end1-start1;
        auto start2 = std::chrono::system_clock::now();
        tuple_of_vectors b = bf(query);             //Brute force method

        auto end2 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds2 = end2-start2;
        //Create the output file

        for (int i=0; i<a.v2.size(); i++){
          if(a.v2[i] == numeric_limits<float>::infinity()) break;
          output_file << "Approximate Nearest neighbor-" <<i<<": "<<ids[a.v1[i]]<<"\n";
          output_file << "True Nearest neighbor-" <<i<<": "<<ids[b.v1[i]]<<"\n";
          output_file << "distanceApproximate: "<<a.v2[i]<<"\n";
          output_file << "distanceTrue: "<<b.v2[i]<<"\n";
          sum1+=a.v2[i];
          sum2+=b.v2[i];
          sum+=sum1/sum2;
        }
        output_file << "tApp: " << elapsed_seconds1.count() << "s\n";
        output_file << "tTrue: " << elapsed_seconds2.count() << "s\n";
        sec1+=elapsed_seconds1.count();
        sec2 +=elapsed_seconds2.count();
      }
      output_file << "tApproximateAverage = " << sec1/number_of_queries << endl;
      output_file << "tTrueAverage = " << sec2/number_of_queries << endl;
      output_file << "MAF = " << (sum)/number_of_queries << endl;
      output_file<<"\n\nEnd of this query file\n\n";
    }
  }
  else if(algorithm.compare("Hypercube")==0)
  {
    std::cout<<"Initializing and storing everything ,this might take a while \n";
    save_vectors();

    create_h_functions();
    while(1){
      if(flag)
      {
        string ans;
        std::cout<<"Do you want to search with a new query_file ( query_file_name / N )\n";
        cin>>ans;
        if(ans.compare("N")==0 || ans.compare("n")==0)break;
        query_file.close();
        if(!file_exists(ans))
        {
          std::cout<<"ERROR Expected name of query_file , given file doesnt exist to terminate type N or n\n";
          continue;
        }
        query_file.open(ans);
      }
      flag=1;
      float sum1 = 0;
      float sum2 = 0;
      float sum = 0;
      long unsigned int number_of_queries = number_of_lines('q');
      std::cout<<"Searching \n";
      for (int i=0; i<number_of_queries; i++) {
        vector <double> query;
        string vec;
        //This piece of code takes all numbers of the query-point and put eache of them in a cell of array words
        vec.assign(vector_data('q',i));
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
        int dimension = ctr;
        query.resize(dimension);
        for (j=0; j<dimension; j++) {
          query[j] = atof(words[j]);
        }
        output_file << "Query: "<<query_ids[i]<<"\n";
        output_file <<"Algorithm: "<<algorithm<<"\n";
        auto start1 = std::chrono::system_clock::now();
        tuple_of_vectors a = knn_Hypercube(query);                    //Searching with Hypercube
        auto end1 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds1 = end1-start1;

        auto start2 = std::chrono::system_clock::now();
        tuple_of_vectors b = bf(query);                   //Searching with Brute Force
        auto end2 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds2 = end2-start2;

          for (int i=0; i<a.v2.size(); i++){
            if(a.v2[i] == numeric_limits<float>::infinity()) break;
            output_file << "Approximate Nearest neighbor-" <<i<<": "<<ids[a.v1[i]]<<"\n";
            output_file << "True Nearest neighbor-" <<i<<": "<<ids[b.v1[i]]<<"\n";
            output_file << "distanceApproximate: "<<a.v2[i]<<"\n";
            output_file << "distanceTrue: "<<b.v2[i]<<"\n";
            sum1+=a.v2[i];
            sum2+=b.v2[i];
            sum+=sum1/sum2;
          }
          output_file << "tApp: " << elapsed_seconds1.count() << "s\n";
          output_file << "tTrue: " << elapsed_seconds2.count() << "s\n";
          sec1+=elapsed_seconds1.count();
          sec2 +=elapsed_seconds2.count();
      }
      output_file << "tApproximateAverage = " << sec1/number_of_queries << endl;
      output_file << "tTrueAverage = " << sec2/number_of_queries << endl;
      output_file << "MAF = " << (sum)/number_of_queries << endl;
      output_file<<"\n\nEnd of this query file\n\n";
    }
  }
  else
  {
  std::cout<<"Initializing and storing everything ,this might take a while \n";
  L = 1;
  create_hash_tables(algorithm);                                         //Create all needed hash tables
  float t;
  int i;
  srand(time(NULL));
  max_dimension = 0;
  for (i = 0; i < L; i++) create_hash_functions(i,algorithm);             //Create  all needed hash functions
  init();
  for (i = 0; i < p.size(); i++) {
    filtered_curves[i] = filter(p[i]);
    t = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/delta));
    snapped_curves[i] = snap(t,filtered_curves[i]);
    if (snapped_curves[i].size() > max_dimension) max_dimension = snapped_curves[i].size();
  }
  vector<vector<double>> key;
  key.resize(p.size());
  for (i = 0; i < p.size(); i++) {
    key[i] = padding(p[i], max_dimension);
    store_to_hash_table(key[i], i);
  }while(1){
    if(flag)
    {
      string ans;
      std::cout<<"Do you want to search with a new query_file ( query_file_name / N )\n";
      cin>>ans;
      if(ans.compare("N")==0 || ans.compare("n")==0)break;
      query_file.close();
      if(!file_exists(ans))
      {
        std::cout<<"ERROR Expected name of query_file , given file doesnt exist to terminate type N or n\n";
        continue;
      }
      query_file.open(ans);
    }
    flag=1;
    std::cout<<"Searching \n";
    float sum1 = 0;
    float sum2 = 0;
    float sum = 0;
    long unsigned int number_of_queries = number_of_lines('q');       //Take the number of total queries
    for (int i=0; i<number_of_queries; i++) {                          //For each query
      vector <double> query;
      string vec;
      vec.assign(vector_data('q',i));                                 //Take the query
      char words[vec.size()][15];
      int j=0, ctr=0;
    //   //This piece of code takes all numbers of the query and put eache of them in a cell of array words
      for(int l=0;l<=vec.size();l++){
        if(vec[l]==' '||vec[l]=='\0' || vec[l]=='\t'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
        }
        else{
          words[ctr][j]=vec[l];
          j++;
        }
      }
      int dimension = ctr;
    //   //Create the query vector-point as a vector
      query.resize(dimension);
      for (j=0; j<dimension; j++) {
        query[j] = atof(words[j]);
      }
    //   //Start the process of the output
      output_file << "Query: "<<query_ids[i]<<"\n";
      output_file <<"Algorithm: LSH_"<<algorithm<<"_"<<metric<<"\n";
      auto start1 = std::chrono::system_clock::now();
      tuple_of_vectors a = knn_cont(query);            //K approximate nearest neighbors
      auto end1 = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds1 = end1-start1;
    //
      auto start2 = std::chrono::system_clock::now();
      tuple_of_vectors b = bf(query);             //Brute force method
      auto end2 = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds2 = end2-start2;
    //
    //   //Create the output file

      for (int i=0; i<a.v2.size(); i++){
        if(a.v2[i] == numeric_limits<float>::infinity()) break;
        output_file << "Approximate Nearest neighbor-" <<i<<": "<<ids[a.v1[i]]<<"\n";
        output_file << "True Nearest neighbor-" <<i<<": "<<ids[b.v1[i]]<<"\n";
        output_file << "distanceApproximate: "<<a.v2[i]<<"\n";
        output_file << "distanceTrue: "<<b.v2[i]<<"\n";
        sum1 += a.v2[i];
        sum2+=b.v2[i];
        sum+=sum1/sum2;
      }
      output_file << "tApp: " << elapsed_seconds1.count() << "s\n";
      output_file << "tTrue: " << elapsed_seconds2.count() << "s\n";
      sec1+=elapsed_seconds1.count();
      sec2 +=elapsed_seconds2.count();
    }
    output_file << "tApproximateAverage = " << sec1/number_of_queries << endl;
    output_file << "tTrueAverage = " << sec2/number_of_queries << endl;
    output_file << "MAF = " << (sum)/number_of_queries << endl;
    output_file<<"\n\nEnd of this query file\n\n";
    }
  }
}
