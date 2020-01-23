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
float extractPerfRat(char* filelist, int angle, int RefFile1, int RefChan1);
int countLines(char *filelist);
float getCorrFactor(int chan, int angle, int iscalib);
ifstream& goToLine(ifstream& file, unsigned int num);
float get_ref_cal(int chan, int angle);


TH1F* makeGSUTree(string f, int iscalib, int angle, int seg)
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
      
      float performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-1] -> GetParameter(1))))*getCorrFactor(i,angleco,iscalib);
      
      cout << "i = " << i << "; Channel = " << chanList[i] << "; PR = " << performance << "; MPV: " << Fits[i] -> GetParameter(1) << endl;
     
      refScale -> Fill(performance);
      positionDep -> SetPoint(i,i,performance);
    }
  
  
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

void Megan_Calibration(char *filelist)
{
   const int numOfFiles = countLines(filelist);
  float minPerf = 4;
  int minPerfChan = 0;
  string minPerfFile = ""; 
  ifstream link; link.open(filelist);
  //float prs[nChans-2][numOfFiles];
  float prs[numOfFiles][nChans-2];
  goToLine(link,0);
  
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
 


      
      for(int k = 1; k < nChans-1 ;k++)
	{
      
	  
	  prs[l][k-1] = (Fits[k] -> GetParameter(1)/(0.5*(Fits[0]->GetParameter(1) + Fits[9]->GetParameter(1))) );
	  //cout << Form("Performance ratio for Test %d, channel %d: %g",l,k,prs[l][k-1]) << endl;
	  
	  
	}
    }


  float chan_1_pr[nChans-2];
  float corr_factor[nChans-2] = {0};
  corr_factor[0] = 1.;
  for(int i = 0; i < numOfFiles ;i++)
    {
      
      chan_1_pr[i] = prs[i][0];
      for(int j = 1; j < nChans - 2; j++)
	{
	  corr_factor[j] += prs[i][j]/prs[i][0];
	}
    }

  for(int j = 1; j < nChans-2; j++)
    {
      corr_factor[j] /= 8;
    }

  float tiles[8];
  for(int i = 0; i < nChans-2;i++)
    {
      if(i == nChans - 3) printf("%g \n",corr_factor[i]);
      else printf("%g,",corr_factor[i]);
      tiles[i] = i+1;
    }

  
  TGraph *factors = new TGraph(nChans-2,tiles,corr_factor);
  factors -> SetMarkerStyle(4);
  factors -> SetTitle(";Channel Num;Correction Factor");
  TCanvas *derp = new TCanvas();
  factors -> Draw("ap");
}


//This is the function that selects new reference tiles.
//At the time of writing, we'll probably need to use it one more time
//for the M series tiles.
float extractPerfRat(char* filelist, int angle, int RefFile1 = 40, int RefChan1 = 40)
{
  const int numOfFiles = countLines(filelist);
  float minPerf = 4;
  int minPerfChan = 0;
  string minPerfFile = ""; 
  ifstream link; link.open(filelist);
  float prs[numOfFiles][nChans-2];
  float calib[nChans-2] = {0};
  goToLine(link,0);
  float globalAve = 0;
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
 


      
      for(int k = 1; k < nChans-1 ;k++)
	{
      
	  
	  prs[l][k-1] = (Fits[k] -> GetParameter(1)/(0.5*(Fits[0]->GetParameter(1) + Fits[9]->GetParameter(1)))) * get_ref_cal(k,angle-20);
	  //cout << Form("Performance ratio for Test %d, channel %d: %g",l,k,prs[l][k-1]) << endl;
	  
	  
	}
    }
  
  /* This was an initial, mistaken calibration
  for(int q = 0; q < nChans-2; q++)
    {
      for(int a = 0; a < numOfFiles; a++)
	{
	  calib[q] += prs[a][q];
	}
      calib[q] /= numOfFiles;
      cout << Form("Calibration factor for channel %d is %g", q,1/calib[q]) << endl;
    }
  
  
  
  for(int i = 0; i < numOfFiles; i++)
    {
      for(int j = 0; j < nChans - 2; j++)
	{
	  globalAve += prs[i][j]*(1/calib[j]);
	}
    }
  globalAve /= numOfFiles*(nChans-2);
  cout << Form("Global average: %g",globalAve) << endl;
  */
  
  for(int g = 0; g < numOfFiles; g++)
    {
      for(int d = 1; d < nChans-1;d++)
	{
	      
	  if((abs(globalAve-prs[d][g]) < minPerf) && g != RefFile1 && d != RefChan1)
	    {
	      minPerfChan = d;
	      goToLine(link,g);
	      link >> minPerfFile;
	      minPerfFile += "filenum:_";
	      minPerfFile += to_string(g);
			     
	      minPerf = abs(globalAve - prs[d][g]);
	      best_perf = prs[d][g];
	    }
	  else
	    {
	      //cout << "Skip!" << endl;
	    }
	}
    }
  cout << "File: " << minPerfFile <<endl;
  cout << "Channel: " << minPerfChan << endl;
  cout << "Global average: " << globalAve << endl;
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
      runningDist -> Add(makeGSUTree(name,iscalib,angle,i));
    }
  runningDist -> Draw();
  runningDist -> SetName(outname);
  output -> cd();
  runningDist -> Write();
  output -> Close();  
}

void makePositDep(char *filelist, int angle, const int fileNum, int iscalib)
{
  
  TMultiGraph *allChans = new TMultiGraph();
  TFile *ins[fileNum];
  ifstream tilelist;
  tilelist.open(filelist);
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  float corrFac[nChans] = {0};
  float corrFacErr[nChans] = {0};
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
      int end = nChans-1;
      
      for(int j = start; j < end; j++)
	{
	  double x,y;
	  dummy -> GetPoint(j,x,y);
	  
	  dummy2 -> SetPoint(j,x,y*getCorrFactor(j,angle-20,iscalib));
	   
	  corrFac[j] += y*getCorrFactor(j,angle-20,iscalib);
	    
	}
      
     
      if(i == 0)  leg -> AddEntry(dummy2,Form("B%d",angle),"p");

      allChans -> Add(dummy2,"p");
      
    }
 
  
  for(int i = 0; i < nChans; i++)
    {
      corrFac[i] /= fileNum;
    }
  float std[nChans] = {0};
  goToLine(tilelist,0);
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
      int end = nChans-1;
      
      for(int j = start; j < end; j++)
	{
	  double x,y;
	  dummy -> GetPoint(j,x,y);
	  
	  dummy2 -> SetPoint(j,x,y*getCorrFactor(j,angle-20,iscalib));
	   
	  //corrFac[j] += y*getCorrFactor(j,angle-20,iscalib);
	  std[j] = pow(corrFac[j] - y,2);
	}
      
     
      if(i == 0)  leg -> AddEntry(dummy2,Form("B%d",angle),"p");

      allChans -> Add(dummy2,"p");
      
    }
  
  
  for(int i = 0; i < nChans; i++)
    {
      std[i] /= fileNum -1;
      std[i] = sqrt(std[i]);
      cout << "Average for position " << i << ": " << corrFac[i] << "\u00b1" << " " << std[i] << endl;
    }

  
  allChans -> Draw("ap");
  allChans -> GetXaxis() -> SetTitle("SiPM Position Number");
  allChans -> GetYaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
  allChans -> GetYaxis() -> SetRangeUser(0,2);
  leg -> Draw("same");

 
}



float getCorrFactor(int chan, int angle, int iscalib)
{

  /*Used and calculated during the beginning of the pre-production
  float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,0.935977,0.918277,0.854018,0.962745,0.870416,0.967691,0.812866,0.873582},
			     {0.847336,0.854614,0.806812,0.769812,0.934153,0.946811,0.864736,0.972421,0.902587,0.88737,0.830939,0.888037},
			     {0.838685,0.821466,0.778553,0.70985,0.893511,0.938996,0.826639,0.968322,0.962354,0.955715,0.922084,0.967667},
			     {0.88497,0.841338,0.788198,0.775766,0.947077,0.938762,0.845317,0.993708,0.961574,1.04429,0.887333,0.966277},
			     {0.838685,0.872622,0.797924,0.831189,0.939296,0.935322,0.921756,0.963874,0.979116,0.82606,0.954908,0.875859},
			     {0.865029,0.887264,0.816234,0.833545,0.97727,0.969677,0.831858,0.985357,0.970586,1.00305,0.864868,0.8912},
			     {0.848827,0.860359,0.791134,0.811859,0.902238,0.900005,0.793583,0.93354,0.918294,0.926915,0.7502,0.934033},
			     {0.831169,0.923422,0.877023,0.817548,0.99918,1.01151,0.9256,0.979267,0.954242,1.00587,0.927643,0.99419}};
  */


  /*Calculated for using all results from the preproduction*/
  /*
  float dualFactor[8][12] = {{1.03729,1.04865,1.00742,1.01741,1,1.10299,1.17016,1.09667,1.19163,1.04526,1.09724,1.13525},
			     {1.01875,1.02049,0.996906,0.98308,1,1.08133,1.1573,1.07222,1.17334,1.04325,1.06025,1.10417},
			     {1.02077,1.00031,1.01602,1.02363,1,1.07813,1.12258,1.09569,1.08201,1.02287,1.03322,1.12255},
			     {1.02371,0.994345,0.987333,0.978207,1,1.0734,1.11627,1.06521,1.09847,0.985456,1.03558,1.08478},
			     {1.01203,1.0011,1.00256,0.975187,1,1.06702,1.10337,1.05535,1.09404,1.01912,1.03313,1.09702},
			     {1.01075,0.999249,0.982315,0.999596,1,1.0611,1.11878,1.04572,1.07822,1.02064,1.03966,1.0764},
			     {1.05119,1.03668,1.02986,1.03021,1,1.09882,1.13749,1.09649,1.12204,1.02832,1.10241,1.12698},
			     {0.988086,0.95988,0.969652,0.939793,1,1.01351,1.02965,1.01127,1.08678,0.955139,1.0206,1.00872}};
  */

  /*Calculated from Megan's method*/

  float dualFactor[8][16] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			     {1.01817,1.02358,1,1,1.03664,1,1,1,1,1,1,1,1,1,1,0.983839},
			     {0.998495,1.02019,1,1,1.01937,1,1,1,1,1,1,1,1,1,1,1.0073},
			     {1.02026,1.03256,1,1,1.03154,1,1,1,1,1,1,1,1,1,1,1.0121},
			     {1.02171,1.02411,1,1,1.01775,1,1,1,1,1,1,1,1,1,1,1.03879},
			     {1.00865,1.03165,1,1,1.02821,1,1,1,1,1,1,1,1,1,1,0.993219},
			     {0.987429,0.987875,1,1,0.976231,1,1,1,1,1,1,1,1,1,1,0.982809},
			     {1.06346,1.06623,1,1,1.06533,1,1,1,1,1,1,1,1,1,1,1.02683}};

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
  float MeanShift[16] = {0.9651,1,1,1,0.8855,1,1,1,1,1,1,1,1,1,1,0.995820};


  if(iscalib)
    {
      return 1;
    }
  else
    {
      return 1/((dualFactor[chan-1][angle-1])*MeanShift[angle-1]);
    }
}


float get_ref_cal(int chan, int angle)
{
  float ref_calib[8][16] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.01552,1.01196},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0.959554,0.96515},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.02547,1.02192},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.02464,1.0105},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0.973989,0.995516},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0.953201,0.969303},
			    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.0227,0.997852}};

  return 1/ref_calib[chan-1][angle-1];
}

int countLines(char *filelist) { 
  int number_of_lines = 0;
  std::string line;
  std::ifstream myfile(filelist);

  while (std::getline(myfile, line))++number_of_lines;
  myfile.seekg (0, ios::beg);
  return number_of_lines;
     
}

ifstream& goToLine(ifstream& file, unsigned int num)
{
  file.seekg(0);
  for(int i = 0; i < num; i++)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
  return file;
}
