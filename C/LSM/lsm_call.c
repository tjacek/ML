#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "lsm.c"

using namespace std;

Samples * readFile(char * filename){
  ifstream infile(filename); // for example
  string line = "";
  int n=0,k=0; 
  while (getline(infile, line)){
    k=0;
    stringstream strstr(line);
    string word = "";
    while (getline(strstr,word, ',')){ 
         //cout << word << '\n';
         k++;
    }
    n++;
  }
  infile.close(); 
  Samples * samples=makeSamples(n,k);
  ifstream infile2(filename);
  int i=0,j=0;
  while (getline(infile2, line)){
    j=0;
    stringstream strstr(line);
    string word = "";
    while (getline(strstr,word, ',')){ 
      double temp = (double)atof(word.c_str());
      if(j==k-1){
	 samples->s[i]->y=temp;
      }else{
         samples->s[i]->x->data[j]=temp;
      }
      j++;   
    }
    i++;
  }
  infile2.close();
  return samples;
} 

void saveFile(double error,double serror,Vector * theta,char * filename){
  FILE *f;
  f = fopen(filename, "a");
  fprintf(f, "error: %f\n",error);
  fprintf(f, "squared error: %f\n", serror);
  for(int i=0;i<theta->n;i++){
     fprintf(f, "%f,",theta->data[i]);
  }
  fprintf(f, "\n");
  fclose(f);
}

void execute(char * trainfile,char * testfile,char * output){
  Samples *train=readFile(trainfile);
  Samples * test=readFile(testfile);
  Vector  * theta=learn(train,0.00001,100.0);
  double error =avg_error(theta,test);
  double serror=mse(theta,test);
  saveFile(error,serror,theta,output);
}

int main(int argc,char * argv[]){
    if(argc<3){
        printf("lsm train test output \n");
        return 1;
    }
    execute(argv[1],argv[2],argv[3]); 
    return 0;
}
