#include "cstdlib"
#include "iostream"
#include "TFile.h"
#include "cstring"
#include "TLegend.h"
#include "TGraph.h"
//#include "TROOT.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TText.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TColor.h"
#include "string"

using namespace std;

int countLines(char *filelist);
float computeSTD(float one, float two, float three);
fstream& goToLine(fstream& file, unsigned int num);


void drawConsistency()
{
  
  fstream input;
  input.open("excelLists/consistency/global.tsv");
  int nLines = countLines("excelLists/consistency/global.tsv");


  goToLine(input,2);
  gStyle -> SetOptStat(1110);
  TH1F *variance = new TH1F("variance","variance",40,-.5,0.5);
  TH1F *two_three = new TH1F("two_three_variance","two_three_variance",40,-0.5,0.5);
  TH1F *one_two = new TH1F("one_two_variance","one_two_variance",40,-0.5,0.5);
  variance -> GetXaxis() ->SetRangeUser(0,0.2);
  one_two -> GetXaxis() -> SetRangeUser(-.20,0.2);
  two_three -> GetXaxis() -> SetRangeUser(-0.2,0.2);
  for(int i = 1; i < nLines; i++)
    {
      goToLine(input, i);
      string xs,ys,zs, ds;
      input >> ds >> ds >> ds >> xs >> ys >> zs;
      float x, y, z;
      x = stof(xs);
      y = stof(ys);
      z = stof(zs);
      
      cout << "x: " << x << " ; y: " << y << " ; z: " << z << " ; i: " << i <<endl;
      float var = computeSTD(x,y,z);
      //cout << var << endl;
      
      one_two -> Fill(x-y);
      
      two_three -> Fill(x-z);
    
      variance -> Fill(var);

      
    }

  variance -> SetTitle(";PR STD;");
  one_two -> SetTitle(";Test 1/2 #Delta;");
  two_three -> SetTitle(";Test 2/3 #Delta;");
  TCanvas *c1 = new TCanvas();
  one_two -> Draw();
  TCanvas *c2 = new TCanvas();
  two_three -> Draw();
  TCanvas *c3 = new TCanvas();
  variance -> Draw();
}


int countLines(char *filelist) { 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}

float computeSTD(float one, float two, float three)
{
  float ave = (one + two + three)/3;
  float ins[3] = {one,two,three};
  float std = 0;
  for(int i = 0; i < 3; i++)
    {
      std += pow(ave - ins[i],2);
    }
  std /= 2;
  return sqrt(std);
  
}

fstream& goToLine(fstream& file, unsigned int num)
{
  file.seekg(0);
  for(int i = 0; i < num; i++)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
  return file;
}
