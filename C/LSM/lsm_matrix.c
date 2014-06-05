#include <stdio.h>
#include <stdlib.h>
#include "lsm.c"
#include <math.h> 

typedef struct Matrix{
   int n;
   int k;
   double ** data;
} Matrix;

void normalize(int i,Matrix *m,Matrix * Id);
Vector * solve(Matrix* m,Vector * v);
void printMatrix(Matrix * m);
void normalize2(int i,Matrix *m,Matrix * r);

Matrix * makeMatrix(int n,int k){
  Matrix * matrix=(Matrix*) malloc(sizeof(Matrix));
  matrix->n=n;
  matrix->k=k;
  matrix->data=(double**) malloc(n*sizeof(double*));
  for(int i=0;i<n;i++){
    matrix->data[i]=(double*) malloc(k*sizeof(double));
    for(int j=0;j<k;j++){
      matrix->data[i][j]=0;
    }
  }
  return matrix;
}

Matrix * identity(int n){
  Matrix * Id=makeMatrix(n,n);
  for(int i=0;i<n;i++){
    Id->data[i][i]=1.0;
  }
  return Id;
}


Matrix * transpose(Matrix * matrix){
  Matrix * trans= makeMatrix(matrix->k,matrix->n);
  for(int i=0;i<trans->n;i++){
    for(int j=0;j<trans->k;j++){
      trans->data[i][j]=matrix->data[j][i];
    }
  }
  return trans; 
}

Vector * multiply(Matrix * m,Vector * v){
  Vector * nv=makeVector(v->n);
  for(int i=0;i<v->n;i++){
    for(int j=0;j<m->k;j++){
      nv->data[i]+= v->data[i] * m->data[i][j];
    }
  }
  return v;
}

Matrix * multiply(Matrix * m1,Matrix * m2){
  Matrix * nm= makeMatrix(m1->n,m2->k);
  for(int i=0;i<nm->n;i++){
    for(int j=0;j<nm->k;j++){
      for(int t=0;t<m1->k;t++){
        nm->data[i][j]=m1->data[i][t]*m2->data[t][j];
      }
    }
  }
  return nm; 
}

Matrix * gauss(Matrix * m){
  int i,j;
  Matrix * r=identity(m->n);
  for(i=0;i<m->n-1;i++){
    normalize(i,m,r);
    double pivot=m->data[i+1][i];
    for(j=0;j<m->n;j++){
      m->data[i+1][j]-=pivot*m->data[i][j];
      r->data[i+1][j]-=pivot*r->data[i][j];
    }
  }

  for(i=m->n-1;0<i;i--){
    normalize2(i,m,r);
    double pivot=m->data[i-1][i];
    for(j=m->n-1;0<=j;j--){
      m->data[i-1][j]-=pivot*m->data[i][j];
      r->data[i-1][j]-=pivot*r->data[i][j];
    }
  }
  //printMatrix(m);
  //printf("$$$$$$$$$$$$$$$\n");
  //printMatrix(r);
  return r;
}

void test_gauss(){
  Matrix * m =makeMatrix(3,3);
  m->data[0][0]=2.0;  m->data[0][1]=-1.0; m->data[0][2]=0.0;
  m->data[1][0]=-1.0; m->data[1][1]=2.0; m->data[1][2]=-1.0;
  m->data[2][0]=0.0;  m->data[2][1]=-1.0; m->data[2][2]=2.0;
  gauss(m);
}

void normalize(int i,Matrix *m,Matrix * r){
  double pivot=m->data[i][i];
  if(pivot!=0){
    for(int j=0;j<m->n;j++){
      m->data[i][j]/=pivot;
      r->data[i][j]/=pivot;
    }
  }
}

void normalize2(int i,Matrix *m,Matrix * r){
  double pivot=m->data[i][i];
  if(pivot!=0){
    for(int j=m->n-1;j>=0;j--){
      m->data[i][j]/=pivot;
      r->data[i][j]/=pivot;
    }
  }
}

Vector * solve(Matrix* m,Vector * v){
  Vector * s=makeVector(v->n);
  for(int i=v->n-1;i>=0; i--){
    s->data[i]=v->data[i];
    for(int j=0;j<v->n; j++){
      s->data[i]-=s->data[i];
    }
  }
  return s;
}

Matrix * extractMatrix(Samples * s){
  Matrix * m= makeMatrix(s->k,s->n);
  for(int i=0;i<m->n;i++){
    for(int j=0;j<m->k;j++){
      m->data[i][j]= s->s[i]->x->data[j];
    }
  }
  return m;
}

Vector * extractVector(Samples * s){
  Vector * v=makeVector(s->n);
  for(int i=0;i<v->n;i++){
    v->data[i]=s->s[i]->y;
  }
  return v;
}

void printMatrix(Matrix * m){
  for(int i=0;i<m->n;i++){
    for(int j=0;j<m->k;j++){
      printf(" %f ",m->data[i][j]);
    }
    printf(" \n");
  }
}

Vector * linearRegression(Samples * s){
  //test_gauss();
  Matrix * x=extractMatrix(s);
  Vector * y=extractVector(s);
  Matrix * x_t=transpose(x);
  Matrix * x_2=multiply(x_t, x); // multiply(x_t, x);
  Matrix * x_r=gauss(x_2);
  Matrix * x_3= multiply(x_r, x_t);
  Vector * theta = multiply(x_3, y);
  //printMatrix(x_3);
  //printf("*****************\n");
  printVector(theta);
  return theta;
}
