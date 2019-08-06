//All this stuff is just setup code from Anthony's makeGSUTree.C

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

//These establish constants and functions used later on in the code. The nBinsH and nBinsT can be used to change number of bins for MPV and Temperature data respectively. The x and y min/max can be used to change dimensions of various plots that use them.

using namespace std;
const int thresh = 250;
const int nChansH = 10;
const int nChansT = 8;
const int nBinsH = 40;
const int nBinsT = 40;
const int xmin = 300, xmax = 800;
const int ymin = 19, ymax = 24;
int countLines(char *filelist);
Double_t makeTempPlots(char *f);
float makeGSUTree(string f, int iscalib);
float getCorrFactor(int chan, int angle);

//This function analyzes the thermocouple data from every channel of one root file and puts it out in a series of histograms. It will also spit out the average temperature through the whole test.

Double_t makeTempPlots(string f)
{
  TFile *inboy = new TFile(f.c_str());
  TTree *tc = (TTree*)inboy -> Get("tc");
  Double_t  temperature[nChansT];
  tc -> SetBranchAddress("temperature",&temperature);

  TCanvas *c1 = new TCanvas();
  c1 -> Divide(4,2);
  float num=0;
  int nEvents = tc -> GetEntries();

  TH1F *temp_plots[nChansT];

  for(int i = 0; i < nChansT; i++)
    {
     
      c1 -> cd(i+1); 
      temp_plots[i] = new TH1F(Form("Chan_%d_temp",i),Form("Chan_%d_temp",i),nBinsT,20,30);
      for(int j = 0; j < nEvents; j++)
	{
	  tc -> GetEntry(j);
	  temp_plots[i] -> Fill(temperature[i]);
	}
      Temp_plots[i] -> Draw();
      num+= temp_plots[i] ->  GetMean();
    }
  cout << "Mean Temperature (C)=" << num/8 << endl;
  return num/8;

  
}

//This portion of the code uses a text file and a couple other functions to analyze the temp and MPV data from multiple root files. The text file used is a list of file locations of every root file being analyzed. For now, this needs to be made by hand, so make sure there are no blank lines or anything that could jack up the code. File locations need to be exact, all the way to the home directory.


void makeTempDists(char *filelist, char *title)
{
  ifstream list;
  int numOfFiles = countLines(filelist);
  cout << numOfFiles << endl;
  list.open(filelist);
  TGraph *TempDist = new TGraph();
  TH2F* MPVPlot = new TH2F("MPVPlot", title, nBinsH, ymin, ymax, nBinsT, xmin, xmax);
  
  for(int i = 0; i < numOfFiles; i++)
    {
      string name;
      list >> name;
      cout << name << endl;
      TempDist -> SetPoint(i+1,i+1.,makeTempPlots(name));
      MPVPlot -> Fill(makeTempPlots(name), makeGSUTree(name,0));
    }
  //This first portion of the graphs produces a TGraph that displays the average temperature of each test as a point. This is useful for mapping change of temperature throughout a day or across a couple days, but for large numbers of files, it isn't much help.
  TCanvas *c1 = new TCanvas();
  TempDist -> SetMarkerStyle(4);
  TempDist -> GetYaxis() -> SetRangeUser(19,24);
  TempDist -> GetYaxis() -> SetTitle("Temperature (C)");
  TempDist -> GetXaxis() -> SetTitle("Test Number");
  TempDist -> Draw("ap");
  TempDist -> SetTitle(title);
  //This second portion of the graphs produces a 2D histogram that displays average MPV vs average Temperature. This is useful for large numbers of fies where a relation between MPV and Temp can be appreciably seen. Smaller numbers, they'll just show up as a few points on the graph.
  TCanvas *c2 = new TCanvas();
  MPVPlot -> SetMarkerStyle(4);
  MPVPlot -> Draw("colz");
  MPVPlot -> GetXaxis() -> SetTitle("Temperature (C)");
  MPVPlot -> GetYaxis() -> SetTitle("MPV");
  MPVPlot -> SetTitle(title);
  
}

//This code just counts how many lines there are in the text file used up above.

int countLines(char *filelist)
{ 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}

//This is Anthony's code that analyzes root files. I've modded it a bit to give the MPV and no extra info. Although, it still opens a lot of canvasses when analyzing a large number of files. That needs to be fixed without breaking the rest of the code.

float makeGSUTree(string f, int iscalib)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalSoftTrig[nChansH];
 
  int nEvents = mppc -> GetEntries();

  int chanList[nChansH] = {11,31,30,29,28,23,22,21,20,10};
  
  TF1 *Fits[nChansH];
  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcalNoTrig histograms.
  //Then it applies an offline coincidence trigger on channels 0 and 1.
  for(int i = 0; i < nChansH; i++)
    {
      // c1 -> cd(i+1);
      gPad -> SetLogy();
      hcalSoftTrig[i] = new TH1F(Form("Chan_%d_Soft_Trigger",chanList[i]),Form("Chan_%d_Soft_Trigger",chanList[i]),250,0,4000);

      for(int j = 0; j < nEvents; j++)
	{
	  mppc -> GetEntry(j);

	  //This cut simply ensures that all hits embedded
	  //a certain amount of energy in the tile above the
	  //chosen threshold ADC of 250
	  
	  int chanCheck = 0;
	  for(int k = 0; k < nChansH; k++)
	    {
	      if(chg[chanList[k]] >= thresh) chanCheck++;
	    }
	  if(chanCheck == nChansH)  hcalSoftTrig[i] -> Fill(chg[chanList[i]]);	  
	  
	}
    
     
      hcalSoftTrig[i] -> Fit("landau","q","+",0,4100);
      Fits[i] = (TF1*)hcalSoftTrig[i] -> GetFunction("landau");
     
    }
  
  int start = 1;
  int end = nChansH - 1;
  
  float MPV=0;
  
  for(int i = 0; i < nChansH ;i++)
    {
      MPV += Fits[i] -> GetParameter(1);
    }
 
 
  return MPV/10.; 
}




//Idk what this part does, but I copied it in case it's needed. Don't think it is, but whatevz.

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
