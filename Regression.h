#ifndef REGRESSION_H
#define REGRESSION_H

#include<iostream>
#include<cmath>
#include<string>

using namespace std;

class Regression {
  public:
    Regression(): _n(0), _x(0), _y(0) {
      _a = 0.0;
      _b = 0.0;
    }

    Regression(const int n, double *x, double *y);

    virtual ~Regression(){}

    double SumX() const;

    double SumX2() const;

    double SumY() const;

    double SumY2() const;

    double SumXY() const;

    double GetParameterA() const {return _a;}

    double GetParameterB() const {return _b;}

    void ShowData();

    void Evaluate();

    double Calculate(double x);

  private: 
    int _n;
    double *_x, *_y;
    double _a, _b;
}; // end of class regression
#endif
