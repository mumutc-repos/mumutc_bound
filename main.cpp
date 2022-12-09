#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Regression.h"

#define SIZE 9

using namespace std;

int main(int argc, char* argv[]) {

int n = 0;

double sqrts = 1.0e4;
double unit = 0.389379;
double lambda = 1.0e4;

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

//fstream ofile_tc;
//ofile.open("output_tc.txt", ios_base::out);
//fstream ofile_bs;
//ofile.open("output_bs.txt", ios_base::out);

return 0;
}
