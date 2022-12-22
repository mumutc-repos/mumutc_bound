#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Regression.h"

#define SIZE 9
#define STEP 10
#define SIG2 2
#define SIG3 3

using namespace std;

int main(int argc, char* argv[]) {

int n = 0;

double sqrts = 1.0e4;
double lambda = 1.0e4;
double lumi = 1.0e4;
double brtc = 0.603;

double bkg_tc = 805.241/3.0;
double bkg_bs = 7991.93/3.0;

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
ifile.open("xs2_clq1_only.txt", ios_base::in);
//ifile.open("xs2decayed_clq1_only.txt", ios_base::in);
//ifile.open("xs2_clq1_eq_3clq3.txt", ios_base::in);

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
    cout<<clq12223[n]<<" "<<clq32223[n]<<" "<<sigma_tc[n]<<" "<<sigma_bs[n]<<endl; 
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
  cout<<"tc: "<<sigma_tc[i]/CLLtc2[i]<<" bs: "<<sigma_bs[i]/CLLbs2[i]<<endl;
}

Regression *reg_tc = new Regression(n, CLLtc2, sigma_tc);
//reg_tc->ShowData();
reg_tc->Evaluate();
cout<<"tc: a="<<reg_tc->GetParameterA()<<" b="<<reg_tc->GetParameterB()<<endl;;

Regression *reg_bs = new Regression(n, CLLbs2, sigma_bs);
//reg_bs->ShowData();
reg_bs->Evaluate();
cout<<"bs: a="<<reg_bs->GetParameterA()<<" b="<<reg_bs->GetParameterB()<<endl;;

for (int i=0; i<n; i++) {
  cout<<"tc: "<<CLLtc[i]<<" "<<reg_tc->Calculate(CLLtc2[i])<<endl;
  cout<<"bs: "<<CLLbs[i]<<" "<<reg_bs->Calculate(CLLbs2[i])<<endl;
}

//output for 1d plots
fstream ofile_tc;
ofile_tc.open("output_tc_clq1_only.txt", ios_base::out);
//ofile_tc.open("output_tc_clq1_eq_3clq3.txt", ios_base::out);

fstream ofile_bs;
ofile_bs.open("output_bs_clq1_only.txt", ios_base::out);
//ofile_bs.open("output_bs_clq1_eq_3clq3.txt", ios_base::out);

double data_begin=-0.04;
double data_end=0.04;
double idata;
double xs;
double ns;
double sig;
double h;

h = (data_end-data_begin)/STEP;

for (int i=0; i<STEP+1; i++) {
  idata = data_begin + i*h;
  xs = reg_tc->Calculate(idata*idata);
  ns = xs*brtc*lumi*cut_tc;
  sig = ns/sqrt(ns+bkg_tc);
  ofile_tc<<idata<<" "<<xs<<" "<<sig<<endl;
}

for (int i=0; i<STEP+1; i++) {
  idata = data_begin + i*h;   
  xs = reg_bs->Calculate(idata*idata);
  ns = xs*lumi*cut_bs;
  sig = ns/sqrt(ns+bkg_bs);
  ofile_bs<<idata<<" "<<xs<<" "<<sig<<endl;
}

//output for 2d plots
fstream ofile_cont;
ofile_cont.open("output_cont_clq1_only.txt", ios_base::out);

double c_begin = -0.04; 
double c_end = 0.04;
//double data_tc[STEP+1][STEP+1], data_bs[STEP+1][STEP+1];
double clq1_in[STEP+1], clq3_in[STEP+1]; 
double clltc_in, cllbs_in;
double xs_tc, xs_bs;
double ns_tc, ns_bs;
double sig_tc, sig_bs;

int label_tc;
int label_bs;

h = (c_end-c_begin)/STEP;

for (int i=0; i<STEP+1; i++) {
  label_tc = 0;
  label_bs = 0;
  clq1_in[i] = c_begin + i*h;
  for(int j=0; j<STEP+1; j++) {
    clq3_in[j] = c_begin + j*h; 
    clltc_in = clq1_in[i] - clq3_in[j];
    cllbs_in = clq1_in[i] + clq3_in[j];
    xs_tc = reg_tc->Calculate(clltc_in*clltc_in);
    xs_bs = reg_bs->Calculate(cllbs_in*cllbs_in);
    ns_tc = xs_tc*brtc*lumi*cut_tc;
    ns_bs = xs_bs*lumi*cut_bs;
    sig_tc = ns_tc/sqrt(ns_tc+bkg_tc);
    sig_bs = ns_bs/sqrt(ns_bs+bkg_bs);
    if(sig_tc>2.0) label_tc = SIG2;
    if(sig_tc>3.0) label_tc = SIG3;
    if(sig_bs>2.0) label_bs = SIG2;
    if(sig_bs>3.0) label_bs = SIG3;
//    ofile_cont<<clq1_in[i]<<" "<<clq3_in[j]<<" "<<xs_tc<<" "<<xs_bs<<" "<<sig_tc<<" "<<sig_bs<<endl;
    ofile_cont<<clq1_in[i]<<" "<<clq3_in[j]<<" "<<label_tc<<" "<<label_bs<<" "<<sig_tc<<" "<<sig_bs<<endl;
  }
}

return 0;
}
