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

  This program analyzes data from the test
  stand currently stationed at GSU. It performs
  a little bit of offline cleanup of the Landau
  distributions from the sPHENIX HCal tiles
  and computes their performance ratios. 
  ----------------------------*/

const int nChans = 10; 
const int thresh = 250;
const int nBins = 40;
TH1F* makeGSUTree(string f, int iscalib, int angle);
void makeRunningDists(char *filelist, int mode);
float extractPerfRat(char* filelist, int mode);
int countLines(char *filelist);
float getCorrFactor(int chan, int angle);

TH1F* makeGSUTree(string f, int iscalib, int angle)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans];
  TH1F *hcalSoftTrig[nChans];
  int angleco = angle - 20; 
 
  int nEvents = mppc -> GetEntries();

  int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};
  
  TCanvas *c1 = new TCanvas();
  c1 -> Divide(4,3);
  TF1 *Fits[nChans];
  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcalNoTrig histograms.
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

	  //This cut simply ensures that all hits embedded
	  //a certain amount of energy in the tile above the
	  //chosen threshold ADC of 250
	  
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
      
      float performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-1] -> GetParameter(1))))*getCorrFactor(i,angleco);
      
      cout << "i = " << i << "; Channel = " << chanList[i] << "; PR = " << performance << "; MPV: " << Fits[i] -> GetParameter(1) << endl;
     
      refScale -> Fill(performance);
      positionDep -> SetPoint(i,i,performance);
    }
  
  
  if(iscalib)
    {
      TFile *output = new TFile(Form("calibFiles/B%dCal_%d.root",angle,rand()%100),"RECREATE");
      positionDep -> SetMarkerStyle(4);
      positionDep -> SetName("positionDep");
      output -> cd();
      positionDep -> Write();
      positionDep -> Draw();
    }
    
  TCanvas *test = new TCanvas();
  refScale -> Draw();
  refScale -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
 
  return refScale;
}


//This is the function that selects new reference tiles.
//At the time of writing, we'll probably need to use it one more time
//for the M series tiles.
float extractPerfRat(char* filelist, int mode, float factor)
{
  int numOfFiles = countLines(filelist);
  float minPerf = 4;
  int minPerfChan = 0;
  string minPerfFile = ""; 
  fstream link; link.open(filelist);
  float best_perf = 0;

  for(int l = 0; l < numOfFiles; l++)
    {
      
      string f;
      link >> f;
      TFile *fin = new TFile(f.c_str());
      TTree *mppc = (TTree*)fin->Get("mppc");
      TH1F *mpvDist = new TH1F("mpvDist","mpvDist",200,400,1400);
      UShort_t chg[32];
      mppc -> SetBranchAddress("chg",&chg);
      TH1F *hcalNoTrig[nChans];
      TH1F *hcalSoftTrig[nChans];
      int nEvents = mppc -> GetEntries();

      int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};
      
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
	    mpvDist -> Fill(Fits[i] -> GetParameter(1));
	  
	}
 
      int start = 1;
    
      for(int k = start; k < nChans-1 ;k++)
	{
      
	  float performance = 0;
	   
	  performance = (1/factor)*(Fits[k] -> GetParameter(1)/Fits[0]->GetParameter(1));

	  if(abs(1-performance) < minPerf)
	    {
	      minPerfChan = k;
	      minPerfFile = f;
	      minPerf = abs(1 - performance);
	      best_perf = performance;
	    }
	  else
	    {
	      cout << "Skip!" << endl;
	    }
	}
    }
  cout << "File: " << minPerfFile <<endl;
  cout << "Channel: " << minPerfChan << endl;
  cout << "Ratio: " << best_perf << endl;
  return 0;
}

void makeRunningDists(char *filelist,  char* outname, int angle, int iscalib=0)
{
  ifstream list;
  int numOfFiles = countLines(filelist);
  cout << numOfFiles << endl;
  list.open(filelist);
  TH1F *runningDist = new TH1F("runningDist","runningDist",nBins,0,2);
  TFile *output = new TFile(Form("%s.root",outname), "RECREATE");
  
  for(int i = 0; i < numOfFiles; i++)
    {
      string name;
      list >> name;
      cout << name << endl;
      runningDist -> Add(makeUniTree(name,iscalib,angle));
    }
  runningDist -> Draw();
  runningDist -> SetName(outname);
  output -> cd();
  runningDist -> Write();
  output -> Close();  
}

void makePositDep(char *filelist, int angle, const int fileNum)
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
      int end = -1;
      
      for(int j = start; j < end; j++)
	{
	    double x,y;
	    dummy -> GetPoint(j,x,y);
	  
	    dummy2 -> SetPoint(j,x,y*getCorrFactor(j,angle-20));
	   
	    corrFac[j] += y*getCorrFactor(j,angle-20);
	    
	    
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
  allChans -> GetYaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
  allChans -> GetYaxis() -> SetRangeUser(0,2);
  leg -> Draw("same");

 
}

float getCorrFactor(int chan, int angle)
{
  
  float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,0.935977,0.918277,0.854018,0.962745,0.870416,0.967691,0.812866,0.873582},
			     {0.847336,0.854614,0.806812,0.769812,0.934153,0.946811,0.864736,0.972421,0.902587,0.88737,0.830939,0.888037},
			     {0.838685,0.821466,0.778553,0.70985,0.893511,0.938996,0.826639,0.968322,0.962354,0.955715,0.922084,0.967667},
			     {0.88497,0.841338,0.788198,0.775766,0.947077,0.938762,0.845317,0.993708,0.961574,1.04429,0.887333,0.966277},
			     {0.838685,0.872622,0.797924,0.831189,0.939296,0.935322,0.921756,0.963874,0.979116,0.82606,0.954908,0.875859},
			     {0.865029,0.887264,0.816234,0.833545,0.97727,0.969677,0.831858,0.985357,0.970586,1.00305,0.864868,0.8912},
			     {0.848827,0.860359,0.791134,0.811859,0.902238,0.900005,0.793583,0.93354,0.918294,0.926915,0.7502,0.934033},
			     {0.831169,0.923422,0.877023,0.817548,0.99918,1.01151,0.9256,0.979267,0.954242,1.00587,0.927643,0.99419}};
  
  return 1/dualFactor[chan-1][angle-1];

}

 
int countLines(char *filelist) { 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}
