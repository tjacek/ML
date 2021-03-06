#include "lsm_matrix.c"
#include <time.h>

Samples * generateSamples(int n,int k);
double eval(Vector * x);
double eps();
Vector * randomVector(Vector * v);
double randomDouble();

void test(int k,int n){
  Samples * train=generateSamples(k*2,n);
  //printSamples(train);
  Samples * test=generateSamples(k,n);
  //test_2();
  //Vector * theta=linearRegression(train);
  //printSamples(test);
   Vector  * theta=learn(train,0.1,0.01);
  printVector(theta);
  double error=mse(theta,train);
  printf("Error %f \n",error);
}

Samples * generateSamples(int k,int n){
    srand(time(NULL));
    Samples * samples=makeSamples(k,n);
    for(int i=0;i<samples->k;i++){
      Sample * s=samples->s[i];
      randomVector(s->x);
      s->y=eval(s->x);
    }
    return samples;    
}

double eval(Vector * x){
  double y;
  double sign=1.0;
  for(int i=0;i<x->n;i++){
    y+=sign*x->data[i];
    sign*=-1.0;
  }
  return y;//+eps();
}

double eval2(Vector * x){
  double y;
  double sign=1.0;
  //for(int i=0;i<x->n;i++){
    y+=x->data[1];
    //sign*=-1.0;
  //}
  return y;//+eps();
}

double eps(){
  return (randomDouble() - 5.0)/10.0;
}

Vector * randomVector(Vector * v){
  for(int i=1;i<v->n;i++){
    v->data[i]=randomDouble();
  }
  return v;
}

double randomDouble(){
   double d=(double) (rand() % 1000);
   return d/100;
}

int main(){
  test(1000,3);
}
