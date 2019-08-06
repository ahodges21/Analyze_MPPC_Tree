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
  This program pulls the MPV of the top and bottom
  reference channels and plots them as a function
  of test number in chronological order (as a function
  of time).
  ----------------------------*/

const int nChans = 10; 
const int thresh = 250;
const int nBins = 40;
double makeGSUTreeMPVTime(string f, int channel, int parameter);
void makeRunningDists(char *filelist, int mode);
float extractPerfRat(char* filelist, int mode);
int countLines(char *filelist);
float getCorrFactor(int chan, int angle);

double makeMPVTime(string f, int channel, int parameter)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans];
  TH1F *hcalSoftTrig[nChans];
  TF1 *fits[246];
 
  int nEvents = mppc -> GetEntries();

  int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};

  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcalNoTrig histograms.
  //Then it applies an offline coincidence trigger on channels 0 and 1.
  for(int i = 0; i < nChans; i++)
    {
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
    }
  hcalSoftTrig[channel] -> Fit("landau","q","0+",0,4100);
  hcalSoftTrig[channel] -> GetYaxis() -> SetRangeUser(1,10e2);
  TF1 *Fits = (TF1*)hcalSoftTrig[channel] -> GetFunction("landau");
  gStyle -> SetOptFit(111);
  
  return Fits->GetParameter(parameter);
}


void makeRunningDists(char *filelist,  char* outname)
{
  TDatime *datemade = new TDatime();
  TFile *output = new TFile(Form("MPV_Time_Output_%d.root",datemade -> GetDate()),"RECREATE");
  
  ifstream rootfilelist;
  int nTests = countLines(filelist);
  cout << nTests << endl;
  rootfilelist.open(filelist);
  TF1 *fit[nTests];
  int channel;
  int parameter;

  TGraphErrors *graph_1 = new TGraphErrors();
  TGraphErrors *graph_2 = new TGraphErrors();
  TGraphErrors *graph_3 = new TGraphErrors();
  
  for(int i = 0; i < 5;i++)
      {
        string name;
	rootfilelist >> name;	
	cout << "Test " << i+1 << " of" << " " << nTests << ": " << name << endl;
	graph_1->SetPoint(i+1,i+1,makeGSUTreeMPVTime(name, 0, 1)); // mpv, top ref
	graph_2->SetPoint(i+1,i+1,makeGSUTreeMPVTime(name, 9, 1)); // mpv, bottom ref
	graph_1->SetPointError(i+1,0,makeGSUTreeMPVTime(name, 0, 2)); // sigma is used for error
	graph_2->SetPointError(i+1,0,makeGSUTreeMPVTime(name, 9, 2)); // sigma is used for error
	graph_3->SetPoint(i+1,i+1,makeGSUTreeMPVTime(name, 9, 1)/makeGSUTreeMPVTime(name, 0, 1)); //bottom/top
      }
			     
  graph_1 -> SetMarkerStyle(4);
  graph_2 -> SetMarkerStyle(4);
  graph_1 -> SetMarkerColor(4);
  graph_1 -> GetXaxis() -> SetTitle("Test Number");
  graph_1 -> GetYaxis() -> SetTitle("MPV");
  graph_1 -> Draw("ap");
  graph_2 -> Draw("samep");
  TLegend* leg = new TLegend(.2,.2,.2,.2);
  leg -> SetTextSize(.04);
  leg -> AddEntry(graph_2,"Bottom Ref MPV");
  leg -> AddEntry(graph_1,"Top Ref MPV");
  leg -> Draw();
  
  TCanvas *c2=new TCanvas();
  graph_3 -> SetMarkerStyle(4);
  graph_3 -> GetXaxis() -> SetTitle("Test Number");
  graph_3 -> GetYaxis() -> SetTitle("MPV Bot Ref/MPV Top Ref");
  graph_3 -> Draw("ap");

  output -> cd();
  graph_1 -> SetName("MPVvsTime_top");
  graph_1 -> Write();
  graph_2 -> SetName("MPVvsTime_bot");
  graph_2 -> Write();
  graph_3 -> SetName("MPVvsTime_ratio");
  graph_3 -> Write();
  output -> Close();
  
}

 
int countLines(char *filelist)
{ 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}

