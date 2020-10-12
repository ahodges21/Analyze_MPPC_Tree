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
TH1F* makeGSUTree(string f, int iscalib, int angle, int seg, const int nMissing);
void makeRunningDists(char *filelist, int mode);
float extractPerfRat(char* filelist, int angle, int RefFile1, int RefChan1);
int countLines(char *filelist);
float getCorrFactor(int chan, int angle, int iscalib);
float get_ref_cal(int chan, int angle);


TH1F* makeGSUTree(string f, int iscalib, int angle, int seg, const int nMissing)
{
 

  int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};
  int chanList_new[nChans-nMissing];
  chanList_new[0] = 11;
  chanList_new[nChans-nMissing-1] = 10;
 
  for(int i = 1; i < nChans-nMissing-1;i++)
    {
      chanList_new[i] = chanList[i];
    }
  /*cross check. Under this convention, you replace the bottom n tiles. 
  for(int i = 0; i < sizeof(chanList_new)/sizeof(chanList_new[0]); i++)
    {
      cout << Form("good channel %d: %d",i+1,chanList_new[i]) <<endl;
    }
  gApplication -> Terminate();
  */
  
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans-nMissing];
  TH1F *hcalSoftTrig[nChans-nMissing];
  int angleco = angle - 20; 
 
  int nEvents = mppc -> GetEntries();

  TCanvas *c1 = new TCanvas();
  c1 -> Divide(4,3);
  TF1 *Fits[nChans-nMissing];
  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcalNoTrig histograms.
  //Then it applies an offline coincidence trigger on channels 0 and 1.
  for(int i = 0; i < nChans-nMissing; i++)
    {
      c1 -> cd(i+1);
      gPad -> SetLogy();
      hcalNoTrig[i] = new TH1F(Form("Chan_%d_Open_Trigger",chanList_new[i]),Form("Chan_%d_Open_Trigger",chanList_new[i]),250,0,4000);
      hcalSoftTrig[i] = new TH1F(Form("Chan_%d_Soft_Trigger",chanList_new[i]),Form("Chan_%d_Soft_Trigger",chanList_new[i]),250,0,4000);

      for(int j = 0; j < nEvents; j++)
	{
	  mppc -> GetEntry(j);
	  hcalNoTrig[i] -> Fill(chg[chanList_new[i]]);

	  //This cut simply ensures that all hits embedded
	  //a certain amount of energy in the tile above the
	  //chosen threshold ADC of 250
	  
	  int chanCheck = 0;
	  for(int k = 0; k < nChans-nMissing; k++)
	    {
	      if(chg[chanList_new[k]] >= thresh) chanCheck++;
	    }
	  if(chanCheck == nChans-nMissing)  hcalSoftTrig[i] -> Fill(chg[chanList_new[i]]);	  
	  
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
  int end = nChans-nMissing - 1;
    cout << Form("Top reference MPV: %g", Fits[0] -> GetParameter(1)) << endl;

  for(int i = start; i < end ;i++)
    {
      
      float performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-nMissing-1] -> GetParameter(1))))*getCorrFactor(i,angleco,iscalib);
      
      //cout << "i = " << i << "; Channel = " << chanList_new[i] << "; PR = " << performance << "; MPV: " << Fits[i] -> GetParameter(1) << endl;
      cout << Form("i = %d; Channel = %d; PR, MPV: - %g - %g",i, chanList[i], performance, Fits[i] -> GetParameter(1)) << endl;
      
      refScale -> Fill(performance);
      positionDep -> SetPoint(i,i,performance);
    }
    cout << Form("Bottom reference MPV: %g", Fits[nChans-nMissing-1]->GetParameter(1)) << endl;

  
  if(iscalib)
    {
      TFile *output = new TFile(Form("calibFiles/B%dCal_%d.root",angle,seg),"RECREATE");
      cout << "output name" << output ->GetName() << endl;
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

float getCorrFactor(int chan, int angle, int iscalib)
{

  /* cout << "channel" << chan << endl;
  cout << "angle " << angle << endl;*/

  /*Calculated from Megan's method*/

  float dualFactor[8][16] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

			     {1.01817,1.02358,1.02595,1.02806,1.03664,1.00424,1.02599,1.04466,1.02418,1.01737,1.01615,1.02268,1.04851,1.02973,1.00965,0.983839},

			     {0.998495,1.02019,1.03223,1.02634,1.01937,1.01391,0.983734,1.02282,0.986605,1.01226,1.0111,1.01303,1.01086,1.03076,0.981761,1.0073},

			     {1.02026,1.03256,1.01491,1.02568,1.03154,1.01067,1.02211,1.03948,1.03135,1.0225,1.01507,1.02949,1.01695,1.07301,0.99169,1.0121},

			     {1.02171,1.02411,0.993743,1.03655,1.01775,1.01989,1.03624,1.05188,1.00428,1.02784,1.02385,1.02885,1.05491,1.07485,1.0424,1.03879},

			     {1.00865,1.03165,1.02716,1.03144,1.02821,1.02497,1.02907,1.05165,1.03411,1.02415,1.02023,1.02713,1.02549,1.02897,0.990605,0.993219},

			     {0.987429,0.987875,0.971813,0.995072,0.976231,0.975675,0.986339,0.997131,0.995258,0.977848,0.984255,0.980894,0.988101,0.981188,0.957596,0.982809},

			     {1.06346,1.06623,1.07072,1.06697,1.06533,1.04125,1.05711,1.07716,1.04637,1.04856,1.04132,1.05508,1.05645,1.05337,1.02704,1.02683}};

  //Really need to clear out some of these defunct correction factors. It's like hoarding, but with git versioning.

  
  
  /*
    float dualFactor[8][12] = {{0.932662,0.957156,1.0051,,,,,,,,,},
    {0.959198,0.981928,1.00981,,,,,,,,,},
    {0.940088,1.00062,1.01009,,,,,,,,,},
    {0.940884,1.00404,1.01967,,,,,,,,,},
    {0.954588,1.00085,1.01337,,,,,,,,,},
    {0.943405,1.00374,1.02564,,,,,,,,,},
    {0.903525,0.967802,0.984223,,,,,,,,,},
    {0.971448,1.04533,1.01663,,,,,,,,,}};
  */ 
  float MeanShift[16] = {0.9651, //21
			 0.9687, //22
			 1.015307156, //23
			 0.826050275, //24
			 0.8855, //25
			 0.937601075, //26
			 0.943933, //27
			 0.895491, //28
			 0.980414, //29
			 1.076, //30
			 1.009, //31
			 0.999813, //32
			 0.921295156, //M49
			 0.852388531, //M50
			 0.977862313, //M51
			 0.95566625}; //M52


  if(iscalib)
    {
      return 1;
    }
  else
    {
      /*cout << "dualFactor: " << ((dualFactor[chan-1][angle-1])) << endl;
	cout << "Meanshift: " << ((MeanShift[angle-1])) << endl;*/
      return 1/((dualFactor[chan-1][angle-1])*(MeanShift[angle-1]));
    }
}

int countLines(char *filelist) { 
  int number_of_lines = 0;
  std::string line;
  std::ifstream myfile(filelist);

  while (std::getline(myfile, line))++number_of_lines;
  myfile.seekg (0, ios::beg);
  return number_of_lines;
     
}


