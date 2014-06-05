#include <stdio.h>
#include <stdlib.h>

typedef struct Vector{
   int n;
   double * data;
} Vector;

typedef struct Sample{
  Vector * x;
  double y;
} Sample;

typedef struct Samples{
  int k;
  int n;
  Sample ** s;
} Samples;

Samples * makeSamples(int k,int n);
Sample * makeSample(int n);
Vector * makeVector(int n);
void clean(Vector * v);
void printVector(Vector * v);

Samples * makeSamples(int k,int n){
  Samples * samples=(Samples*) malloc(sizeof(Samples));
  samples->s=(Sample**) malloc(k*sizeof(Sample*));
  samples->k=k;
  samples->n=n;
  for(int i=0;i<k;i++){
    samples->s[i]=makeSample(samples->n);
  }
  return samples;
}

Sample * makeSample(int n){
  Sample * s=(Sample*) malloc(sizeof(Sample));
  s->x=makeVector(n);
  s->x->data[0]=1.0;
  s->y=0.0; 
  return s;
}

Vector * makeVector(int n){
    Vector * v=(Vector*) malloc(sizeof(Vector));
    v->n=n;
    v->data= (double*)malloc(n*sizeof(double));
    clean(v);
    return v;
}

void clean(Vector * v){
  int i;
  for(i=0;i<v->n;i++){
    v->data[i]=0.0;
  }
}

void printVector(Vector * v){
   for(int i=0;i<v->n;i++){
      printf(" %f",v->data[i]);
   }
   printf("\n");
}

void printSamples(Samples * samples){
   for(int i=0;i<samples->k;i++){
      Sample* s=samples->s[i];
      printf("y:%f x:",s->y);
      printVector(s->x);
   }
   printf("\n");
}
