#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Regression.h"

#define SIZE 9
#define STEP 100

using namespace std;

int main(int argc, char* argv[]) {

int n = 0;

double sqrts = 1.0e4;
double lambda = 1.0e4;
double lumi = 1.0e4;

double bkg_tc = 805.241;
double bkg_bs = 7991.93;

double cut_tc = 0.11945;
double cut_bs = 0.13965;

double clq12223[SIZE];
double clq32223[SIZE];

double CLLtc[SIZE];
double CLLbs[SIZE];

double CLLtc2[SIZE];
double CLLbs2[SIZE];

double sigma_tc[SIZE];
double sigma_bs[SIZE];

//Read parameters from input file
fstream ifile;
ifile.open("xs2.txt", ios_base::in);

if(!ifile.is_open()) {
  cout<<"Input file does not exist!"<<endl;
  return 0;
}

string line;

while(getline(ifile, line)) {
  if(!line.empty()) {
    size_t pos = line.find("#");
    if(pos == 0) continue;
    istringstream iss(line);
    iss>>clq12223[n]>>clq32223[n]>>sigma_tc[n]>>sigma_bs[n]; 
//    cout<<clq12223[n]<<" "<<clq32223[n]<<" "<<sigma_tc[n]<<" "<<sigma_bs[n]<<endl; 
  }
  n++;
}

ifile.close();

//Do linear regression
cout<<n<<" points"<<endl;

for(int i=0; i<n; i++) {
  CLLtc[i] = clq12223[i]-clq32223[i];  
  CLLbs[i] = clq12223[i]+clq32223[i];  
  CLLtc2[i] = CLLtc[i]*CLLtc[i];
  CLLbs2[i] = CLLbs[i]*CLLbs[i];
//  cout<<"tc: "<<sigma_tc[i]/CLLtc2[i]<<" bs: "<<sigma_bs[i]/CLLbs2[i]<<endl;
}

Regression *reg_tc = new Regression(n, CLLtc2, sigma_tc);
//reg_tc->ShowData();
reg_tc->Evaluate();
cout<<"tc: a="<<reg_tc->GetParameterA()<<" b="<<reg_tc->GetParameterB()<<endl;;

Regression *reg_bs = new Regression(n, CLLbs2, sigma_bs);
//reg_bs->ShowData();
reg_bs->Evaluate();
cout<<"bs: a="<<reg_bs->GetParameterA()<<" b="<<reg_bs->GetParameterB()<<endl;;

fstream ofile_tc;
ofile_tc.open("output_tc.txt", ios_base::out);

fstream ofile_bs;
ofile_bs.open("output_bs.txt", ios_base::out);

double data_begin=-0.05;
double data_end=0.05;
double idata;
double xs;
double ns;
double sig;
double h;

h = (data_end-data_begin)/STEP;

for (int i=0; i<STEP+1; i++) {
  idata = data_begin + i*h;
  xs = reg_tc->Calculate(idata*idata);
  ns = xs*lumi*cut_tc;
  sig = ns/sqrt(ns+bkg_tc);
  ofile_tc<<idata<<" "<<sig<<endl;
}

for (int i=0; i<STEP+1; i++) {
  idata = data_begin + i*h;   
  xs = reg_bs->Calculate(idata*idata);
  ns = xs*lumi*cut_bs;
  sig = ns/sqrt(ns+bkg_bs);
  ofile_bs<<idata<<" "<<sig<<endl;
}

return 0;
}
