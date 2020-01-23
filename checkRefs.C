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

const int nChans = 10; 
const int thresh = 250;
int countLines(char *filelist);
void checkRefs(string f, int angle);

void checkRefs(int angle, char *filelist)
{
  fstream list; list.open(filelist);
  const int nLines = countLines(filelist);
  float chan11PR = 0;
  float chan10PR = 0;
  for(int q = 0; q < nLines; q++)
    {
      string f; list >> f;
      TFile *fin = new TFile(f.c_str());
      TTree *mppc = (TTree*)fin->Get("mppc");
      UShort_t chg[32];
      mppc -> SetBranchAddress("chg",&chg);
      TH1F *hcalSoftTrig[nChans];
      int angleco = angle - 20; 
      float MeanShift[16] = {0.9651,0.9687,1.015307156,1,0.8855,1,1,1,1,1,1,1,0.921295156,0.852388531,0.977862313,0.95566625};
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
	  hcalSoftTrig[i] = new TH1F(Form("Chan_%d_Soft_Trigger",chanList[i]),Form("Chan_%d_Soft_Trigger",chanList[i]),250,0,4000);

	  for(int j = 0; j < nEvents; j++)
	    {
	      mppc -> GetEntry(j);

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

      chan11PR += Fits[0] -> GetParameter(1)/(0.5*(Fits[0]->GetParameter(1) + Fits[9]->GetParameter(1)))*(1/MeanShift[angleco-1]);
      chan10PR += Fits[9] -> GetParameter(1)/(0.5*(Fits[0]->GetParameter(1) + Fits[9]->GetParameter(1)))*(1/MeanShift[angleco-1]);;

      
    }
  chan11PR /= nLines;
  chan10PR /= nLines;
  cout << Form("Channel 11 PR: %g", chan11PR) << endl;
  cout << Form("Channel 10 PR: %g", chan10PR) << endl;
}

int countLines(char *filelist) { 
  int number_of_lines = 0;
  std::string line;
  std::ifstream myfile(filelist);
    
  while (std::getline(myfile, line))++number_of_lines;
  myfile.seekg (0, ios::beg);
  return number_of_lines;
  
}
