#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<cmath>

#include "Regression.h"

using namespace std;

Regression::Regression(const int n, double *x, double *y) {
  _n = n;
  _x = x;
  _y = y;
  _a = 0.0;
  _b = 0.0;
}

double Regression::SumX() const {
  double sum = 0;
  for (int i=0; i<_n; i++) {
    sum = sum + _x[i];
  }
  return sum;
}

double Regression::SumY() const {
  double sum = 0;
  for (int i=0; i<_n; i++) {
    sum = sum + _y[i];
  }
  return sum;
}

double Regression::SumX2() const {
  double sum = 0;
  for (int i=0; i<_n; i++) {
    sum = sum + _x[i]*_x[i];
  }
  return sum;
}

double Regression::SumY2() const {
  double sum = 0;
  for (int i=0; i<_n; i++) {
    sum = sum + _y[i]*_y[i];
  }
  return sum;
}

double Regression::SumXY() const {
  double sum = 0;
  for (int i=0; i<_n; i++) {
    sum = sum + _x[i]*_y[i];
  }
  return sum;
}

void Regression::ShowData() {
  for(int i=0; i<_n; i++) {
   cout<<"Point"<<i<<": "<<_x[i]<<" "<<_y[i]<<endl;
  }
}

void Regression::Evaluate() {
  double sumx, sumy;
  double sumx2, sumy2;
  double sumxy;

  sumx = this->SumX();
  sumx2 = this->SumX2();
  sumy = this->SumY();
  sumy2 = this->SumY2();
  sumxy = this->SumXY();
  _b = (_n*sumxy-sumx*sumy)/(_n*sumx2-sumx*sumx);
  _a = (sumy-_b*sumx)/_n;
}
