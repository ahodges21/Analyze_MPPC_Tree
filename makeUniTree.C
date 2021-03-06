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

/*-------------------------

  This program analyzes the data sent in from
  Uniplast from the BNL Tile Tester currently stationed there.
  ----------------------------*/

const int nChans = 10; //Currently testing ten at a time
const int thresh = 475;//475 for Uniplast
const int nBins = 40;
TH1F* makeUniTree(string f, int iscalib, int angle, int seg);
void makeRunningDists(char *filelist, char* outname, int angle, int iscalib = 0);
int countLines(char *filelist);
float getCorrFactor(int chan, int angle, int iscalib);

TH1F* makeUniTree(string f, int iscalib, int angle, int seg = 0)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans];
  TH1F *hcalSoftTrig[nChans];
  int angleco = angle - 20; 
 
  int nEvents = mppc -> GetEntries();

  int chanList[nChans] = {0,2,3,8,9,10,11,12,13,1}; //Uniplast Channel List

  TCanvas *c1 = new TCanvas();
  c1 -> Divide(4,3);
  TF1 *Fits[nChans];
  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcanNoTrig histograms.
  //Then it applies an offline coincidence trigger on channels 0 and 1.
  for(int i = 0; i < nChans; i++)
    {
      c1 -> cd(i+1);

      gPad -> SetLogy();
      hcalNoTrig[i] = new TH1F(Form("Chan_%d_Open_Trigger",chanList[i]),Form("Chan_%d_Open_Trigger",chanList[i]),250,0,4000);
      hcalSoftTrig[i] = new TH1F(Form("Chan_%d_Soft_Trigger",chanList[i]),Form("Chan_%d_Soft_Trigger",chanList[i]),250,0,4000);
      
      for(int j = 0; j < nEvents; j++)
	{
	  mppc -> GetEntry(j);
	  hcalNoTrig[i] -> Fill(chg[chanList[i]]);
	  

	  //John H's method
	  
	  int chanCheck = 0;
	  for(int k = 0; k < nChans; k++)
	    {
	      if(chg[chanList[k]] >= thresh) chanCheck++;
	    }
	  if(chanCheck == nChans)  hcalSoftTrig[i] -> Fill(chg[chanList[i]]);
	}
      
      
      hcalSoftTrig[i] -> Fit("landau","q","+",0,4100);
      hcalSoftTrig[i] -> GetYaxis() -> SetRangeUser(1,10e2);
      Fits[i] = (TF1*)hcalSoftTrig[i] -> GetFunction("landau");
      gStyle -> SetOptFit(111);

    
      
      
      hcalSoftTrig[i] -> Draw();
    }

 
  TCanvas *c3 = new TCanvas();
  TH1F *refScale = new TH1F("refScale","refScale",nBins,0,2);
  positionDep = new TGraph();
  int start = 1;
  int end = nChans - 1;
  for(int i = start; i < end ;i++)
    {
           
      float performance = 0;
      performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-1] -> GetParameter(1))))*getCorrFactor(i-1,angleco,iscalib);
      cout << "i = " << i << "; Channel = " << chanList[i] << "; PR = " << performance << "; MPV: " << Fits[i] -> GetParameter(1) << endl;
      refScale -> Fill(performance);
      positionDep -> SetPoint(i,i,performance);
    }
  
  
  if(iscalib)
    {
      TFile *output = new TFile(Form("calibMfiles/B%dCal_%d.root",angle,seg),"RECREATE");
      positionDep -> SetMarkerStyle(4);
      positionDep -> SetName("positionDep");
      output -> cd();
      positionDep -> Write();
      positionDep -> Draw();
    }
    
  TCanvas *test = new TCanvas();
  refScale -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");

  refScale -> Draw();
 
  return refScale;
}


void makeRunningDists(char *filelist, char* outname, int angle, int iscalib=0)
{
  
  ifstream list;
  int numOfFiles = countLines(filelist);
  cout << numOfFiles << endl;
  list.open(filelist);
  TH1F *runningDist;
  TFile *output = new TFile(Form("%s.root",outname), "RECREATE");
  runningDist = new TH1F("Performance Distribution","Performance Distribution",nBins,0,2);
  
  for(int i = 0; i < numOfFiles; i++)
    {
      string name;
      list >> name;
      cout << name << endl;
      runningDist -> Add(makeUniTree(name,iscalib,angle,i));
    }
  runningDist -> Draw();
  runningDist -> SetName(outname);
  runningDist ->GetXaxis()->SetTitle("MPV_{Tile}/<MPV_{Refs}>");
  
  output -> cd();
  runningDist -> Write();
  output -> Close();
}


int countLines(char *filelist) { 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}


void makePositDep(char *filelist, int angle, const int fileNum, int iscalib)
{
  
  TMultiGraph *allChans = new TMultiGraph();
  TFile *ins[fileNum];
  ifstream tilelist;
  tilelist.open(filelist);
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  float corrFac[nChans] = {0};
  int added[4] = {0};
  float tileCorr = 0;
  for(int i = 0; i < fileNum; i++)
    {
      string name;
      tilelist >> name;
      cout << name << endl;
      ins[i] = new TFile(name.c_str());
      TGraph *dummy = (TGraph*)ins[i] -> Get("positionDep");
      TGraph *dummy2 = new TGraph();
      dummy2 -> SetMarkerStyle(4);
      dummy2 -> GetYaxis() -> SetRangeUser(0,2);
      int start = 1;
      int end = nChans - 1;
      
      for(int j = start; j < end; j++)
	{
	    double x,y;
	    dummy -> GetPoint(j,x,y);
	  
	    dummy2 -> SetPoint(j,x,y*getCorrFactor(j,angle-20, iscalib));
	    
	    corrFac[j] += y*getCorrFactor(j,angle-20, iscalib);
	    
	}
      
   
      if(i == 0)  leg -> AddEntry(dummy2,Form("B%d",angle),"p");

      allChans -> Add(dummy2,"p");
      
    }
 
  
  for(int i = 0; i < nChans; i++)
    {
      corrFac[i] /= fileNum;
      cout << "Average for position " << i << ": " << corrFac[i] << endl;
    }
  
  
  
  allChans -> Draw("ap");
  allChans -> GetXaxis() -> SetTitle("SiPM Position Number");
  allChans -> GetYaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
  allChans -> GetYaxis() -> SetRangeUser(0,2);
  leg -> Draw("same");

 
}

 
float getCorrFactor(int chan, int angle, int iscalib)
{
  
  float uniFactor[8][16] = {{1.07134,1.02745,1,1,1,1,1,1,1,1,1,1,1.07728,1.16182,0.967205,1.00542},
			    {0.97717,0.948518,1,1,1,1,1,1,1,1,1,1,1.02194,1.04401,0.808341,0.944358},
			    {0.984534,0.982921,1,1,1,1,1,1,1,1,1,1,1.02504,1.15625,0.89641,0.950605},
			    {1.02608,1.00779,1,1,1,1,1,1,1,1,1,1,1.05233,1.11478,0.956458,0.968951},
			    {1.03199,1.03054,1,1,1,1,1,1,1,1,1,1,1.09404,1.15629,0.950297,0.961107},
			    {1.04987,1.05428,1,1,1,1,1,1,1,1,1,1,1.09405,1.19737,1.00274,1.02314},
			    {1.02072,1.01345,1,1,1,1,1,1,1,1,1,1,1.05458,1.20478,0.956957,0.953656},
			    {0.975692,0.992402,1,1,1,1,1,1,1,1,1,1,1.00196,1.1494,0.926969,0.938573}}; //I have no idea how many tile angles there actually are anymore, there are at least 14 though, based on the excel documents. 
  

  if(iscalib)
    {
      return 1;
    }
  else
    {
      
      return 1/uniFactor[chan][angle-1];
    }
}

 

