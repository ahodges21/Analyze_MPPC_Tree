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
  Because it's triggering on all channel pairs, 
  I will impose an offline software trigger
  ----------------------------*/

const int nChans = 10; //10 for Uniplast data, 9 for first shipment preliminary testing
const int Trig1 = 0;//10; //0 for uniplast data
const int Trig2 = 1;//11; //1 for uniplast data
const int thresh = 250;//250 for GSU, 475 for Uniplast
const int setup = 5;
/*----------------
Correction factor setup list: 
Setup 3 for uniplast data
Serup 5 for dual reference tile setup
------------*/
const int strl = 72;
const int nBins = 40;
TH1F* makeUniTree(string f, int mode, int iscalib);
void makeRunningDists(char *filelist, int mode);
float extractPerfRat(char* filelist, int mode);
float extractSBR(TH1F *histin);
int countLines(char *filelist);
float getCorrFactor(int chan, int setup, int angle);
void superImpose(int start, char* globalname = "", char* bd = "");
//Mode 1 makes a distribution stack MPV's from the file list provided
//Mode 2 makes a graph that plots the average stack MPV versus the test
//number to track the performance over time. 

TH1F* makeUniTree(string f, int mode, int iscalib, int angle)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  TH1F *mpvDist = new TH1F("mpvDist","mpvDist",200,400,1400);
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans];
  TH1F *hcalSoftTrig[nChans];
  int angleco = angle - 20; 
  
 
  
    

  int nEvents = mppc -> GetEntries();

  //int chanList[nChans] = {0,1,2,3,8,9,10,11,12,13}; //Uniplast Channel List

  //int chanList[nChans] = {0,1,2,3,4,5,6,7}; // GSU Channel List

  //int chanList[nChans] = {2,3,4,5,6,7};

  //int chanList[nChans] = {0,1,2,3,8,9}; //Uniplast list without inner tiles from pilot tests

  //int chanList[nChans] = {0,1,2,3,4,5,6,7,8}; //GSU test stand for first batch of pre-production tiles

  int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};

  //int chanList[nChans] = {31,30,29,28,23,22,21,20,10};
  
  TCanvas *c1 = new TCanvas();
  c1 -> Divide(4,3);
  TF1 *Fits[nChans];
  float mpvChan0 = 0;
  float mpvChan1 = 0;
  float mpvAve = 0;
  float sbr[nChans];
  int index[nChans];
  TGraph *positionDep;
 
  //The code here compiles the raw data from each channel into the hcanNoTrig histograms.
  //Then it applies an offline coincidence trigger on channels 0 and 1.
  for(int i = 0; i < nChans; i++)
    {
      c1 -> cd(i+1);
      index[i] = i;
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
	  
	    
	  
	  
	  //Conventional coincidence triggering
	  /*
	  if(chg[Trig1] >= thresh && chg[Trig2] >= thresh)
	    {
	      hcalSoftTrig[i] -> Fill(chg[chanList[i]]);
	    }
	  */
	  
	 
	  
	  
	}
      
      if(hcalSoftTrig[i] -> Integral(hcalSoftTrig[i] -> GetXaxis() -> FindBin(1000),hcalSoftTrig[i] -> GetXaxis() -> FindBin(2000))>1){
	hcalSoftTrig[i] -> Fit("landau","q","+",0,4100);
	hcalSoftTrig[i] -> GetYaxis() -> SetRangeUser(1,10e2);
	//sbr[i] = extractSBR(hcalSoftTrig[i]);
	Fits[i] = (TF1*)hcalSoftTrig[i] -> GetFunction("landau");
	gStyle -> SetOptFit(111);
	mpvDist -> Fill(Fits[i] -> GetParameter(1));
	mpvAve += Fits[i] -> GetParameter(1);
      }
      
      
      if(mode == 0)hcalSoftTrig[i] -> Draw();
     
     
    
     
    }

  mpvAve /= nChans;
  TCanvas *c3 = new TCanvas();
  TH1F *refScale = new TH1F("refScale","refScale",nBins,0,2);
  positionDep = new TGraph();
  int start = 1;
  int end = nChans - 1;
  if(setup  == 3)
    {
      start = 0;
      end = nChans;
    }
  for(int i = start; i < end ;i++)
    {
      
      
      float performance = 0;
      if(mode == 2) performance = (Fits[i] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(i,setup,angleco);
      if(mode == 3) performance = (Fits[i] -> GetParameter(1)/mpvAve)*getCorrFactor(i,setup,angleco);
      if(mode == 4) performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-1] -> GetParameter(1))))*getCorrFactor(i,setup,angleco);
      //if(performance <= 0.8 || performance >= 1.2){
      cout << "i = " << i << "; Channel = " << chanList[i] << "; PR = " << performance << "; MPV: " << Fits[i] -> GetParameter(1) << endl;
      //}
      refScale -> Fill(performance);
      positionDep -> SetPoint(i,i,performance);
	/*
	  if((Fits[i] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(i,1,angleco) <0.8 || (Fits[i] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(i,1,angleco) > 1.2)
	  {
	  cout << "File: " << f << endl;
	  cout << "Channel" << i << endl;
	  cout << "Ratio: " << (Fits[i] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(i,1,1) << endl;
	  }*/
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
  if(mode == 3)refScale -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
  if(mode == 2)refScale -> GetXaxis() -> SetTitle("MPV_{Tile}/MPV_{Ref}");
  refScale -> Draw();
  if(mode == 4)refScale -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
 
  if(mode == 2 || mode == 3 || mode == 4) return refScale;
  
  return 0;
}

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
      //string angle = f.substr(71,2);
      
      // stringstream convert(angle);
      //int angleco = 0; convert >> angleco;
      
      //angleco -= 20;
      TFile *fin = new TFile(f.c_str());
      TTree *mppc = (TTree*)fin->Get("mppc");
      TH1F *mpvDist = new TH1F("mpvDist","mpvDist",200,400,1400);
      UShort_t chg[32];
      mppc -> SetBranchAddress("chg",&chg);
      TH1F *hcalNoTrig[nChans];
      TH1F *hcalSoftTrig[nChans];
      int nEvents = mppc -> GetEntries();

      //int chanList[nChans] = {0,1,2,3,8,9,10,11,12,13}; //Uniplast Channel List

      //int chanList[nChans] = {0,1,2,3,4,5,6,7}; // GSU Channel List

      //int chanList[nChans] = {2,3,4,5,6,7};

      //int chanList[nChans] = {0,1,2,3,8,9}; //Uniplast list without inner tiles from pilot tests

      //int chanList[nChans] = {0,1,2,3,4,5,6,7,8}; //GSU test stand for first batch of pre-production tiles

      int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};

      //int chanList[nChans] = {31,30,29,28,23,22,21,20,10};
      
      TCanvas *c1 = new TCanvas();
      c1 -> Divide(4,3);
      TF1 *Fits[nChans];
      float mpvChan0 = 0;
      float mpvChan1 = 0;
      float mpvAve = 0;
      float sbr[nChans];
      int index[nChans];
      TGraph *positionDep;
 
      //The code here compiles the raw data from each channel into the hcanNoTrig histograms.
      //Then it applies an offline coincidence trigger on channels 0 and 1.
      for(int i = 0; i < nChans; i++)
	{
	  c1 -> cd(i+1);
	  index[i] = i;
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
	  
	    
	  
	  
	      //Conventional coincidence triggering
	      /*
		if(chg[Trig1] >= thresh && chg[Trig2] >= thresh)
		{
		hcalSoftTrig[i] -> Fill(chg[chanList[i]]);
		}
	      */
	  
	 
	  
	  
	    }
      
	  if(hcalSoftTrig[i] -> Integral(hcalSoftTrig[i] -> GetXaxis() -> FindBin(1000),hcalSoftTrig[i] -> GetXaxis() -> FindBin(2000))>1){
	    hcalSoftTrig[i] -> Fit("landau","q","+",0,4100);
	    hcalSoftTrig[i] -> GetYaxis() -> SetRangeUser(1,10e2);
	    //sbr[i] = extractSBR(hcalSoftTrig[i]);
	    Fits[i] = (TF1*)hcalSoftTrig[i] -> GetFunction("landau");
	    gStyle -> SetOptFit(111);
	    mpvDist -> Fill(Fits[i] -> GetParameter(1));
	    mpvAve += Fits[i] -> GetParameter(1);
	  }
      
      
     
      
	  if(mode == 0)hcalSoftTrig[i] -> Draw("");
        
    
     
	}
 
      mpvAve /= nChans;
      int start;
      if(mode == 4)
	{
	  start = 0;
	}
      else
	{
	  start = 1;
	}
  
      for(int k = start; k < nChans-1 ;k++)
	{
      
	  float performance = 0;
	   
      
	  if(mode == 2)
	    {
	      performance = (1/factor)*(Fits[k] -> GetParameter(1)/Fits[0]->GetParameter(1));//*getCorrFactor(k,5,angleco);
	    }
	  else if(mode == 3)
	    {
	      performance = (Fits[k] -> GetParameter(1)/mpvAve);//*getCorrFactor(k,5,angleco);
	    }
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
	      /*
	  cout << "Filename check: " << minPerfFile << endl;
	  cout << "Best Performance: " << minPerf << endl;
	  cout << "Performance ratio: " << performance << endl;
	      */
	}
      

  
    }
  cout << "File: " << minPerfFile <<endl;
  cout << "Channel: " << minPerfChan << endl;
  cout << "Ratio: " << best_perf << endl;
  return 0;
}
  



void makeRunningDists(char *filelist, int mode, char* outname, int angle, int iscalib=0)
{
  
  ifstream list;
  int numOfFiles = countLines(filelist);
  cout << numOfFiles << endl;
  list.open(filelist);
  cout << "Test" << endl;
  TH1F *runningDist;
  TFile *output = new TFile(Form("%s.root",outname), "RECREATE");
  runningDist = new TH1F("runningDist","runningDist",nBins,0,2);
  
  for(int i = 0; i < numOfFiles; i++)
    {
      string name;
      list >> name;
      cout << name << endl;
      runningDist -> Add(makeUniTree(name,mode,iscalib,angle));
    }
  runningDist -> Draw();
  runningDist -> SetName(outname);
  
  output -> cd();
  runningDist -> Write();//Use save = 0 if you need to debug
  output -> Close();
 

 
  
}

float extractSBR(TH1F *histin)
{
  histin -> Fit("landau","","+",250,4100);
  histin -> Fit("gaus","","+",0,250);

  TF1 *land = (TF1*)histin -> GetFunction("landau");
  TF1 *gauss = (TF1*)histin -> GetFunction("gauss");

  float signal = land->Integral(land->GetParameter(1)-land->GetParameter(2),land->GetParameter(1)+land->GetParameter(2));
  float backg = gauss -> Integral(0,250);

  return signal/backg;
}

int countLines(char *filelist) { 
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;
     
}

const int numSec = 4;
void superImpose(int start, char* globalname = "", char* bd = "")
{
  gStyle -> SetOptStat(1111);
  TH1F *hists[numSec];
  TFile *secs[numSec];
  
  if(globalname){
    TFile *global = new TFile(Form("%s.root",globalname));
    TH1F *allSecs = (TH1F*)global->Get(globalname);
    allSecs -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
    TCanvas *globalc = new TCanvas();
    allSecs -> SetTitle("");
    allSecs -> SetName("All Tiles");
    allSecs -> Draw();
  }
  int colors[4] = {1,2,3,6};
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  //leg -> AddEntry(allSecs,"All Tiles","l");
  TCanvas *breakdown = new TCanvas();
  for(int i = start-1; i < numSec+start-1; i++)
    {
      secs[i-start+1] = new TFile(Form("B%d%s.root",i+21,bd));
      cout << Form("B%d%s",i+21,bd) << endl;
      hists[i-start+1] = (TH1F*)secs[i-start+1] -> Get(Form("B%d%s",i+21,bd));
      //hists[i] -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
      hists[i-start+1] -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
      hists[i-start+1] -> SetName(Form("B%d",i+21));
      hists[i-start+1] -> SetLineColor(colors[i-start+1]);
      hists[i-start+1] -> SetMarkerStyle(4);
      hists[i-start+1] -> SetTitle("");
      leg -> AddEntry(hists[i-start+1],Form("B%d",i+21),"l");
    }
  leg -> Draw("same");
 
  TCanvas *c1 = new TCanvas();
  c1 -> Divide(2,2);
  for(int i = 0; i < numSec; i++)
    {
      c1 -> cd(i+1);
      hists[i] -> GetXaxis() -> SetRangeUser(0,2);
	
      hists[i] ->Draw();
	
    }
  
}

const int fileNum = 3;
void makePositDep(char *filelist, int setup, int angle)
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
      int start;
      int end;
      if(setup == 3){
	start = 0;
	end = nChans;
      }
      if(setup == 5){
	start = 1;
	end = nChans -1;
      }
      
      for(int j = start; j < end; j++)
	{
	  // if(j != 0 && j!=9){
	    double x,y;
	    dummy -> GetPoint(j,x,y);
	  
	    dummy2 -> SetPoint(j,x,y*getCorrFactor(j,setup,angle-20));
	    /*
	      else{
	      dummy -> SetPoint(j+1,x,y*getCorrFactor(j,setup));
	      }
	    */
	    corrFac[j] += y*getCorrFactor(j,setup,angle-20);
	    // }
	    
	}
      
      /*
      //check to see what color to make the markers
      if(!strcmp(code.c_str(),"B21"))
	{
	  dummy2 -> SetMarkerColor(1);
	  
	}
      if(!strcmp(code.c_str(),"B22"))
	{
	  dummy2 -> SetMarkerColor(2);
	  
	}      
      if(!strcmp(code.c_str(),"B23"))
	{
	  dummy2 -> SetMarkerColor(3);
	}
      if(!strcmp(code.c_str(),"B24"))
	{
	  dummy2 -> SetMarkerColor(6);
	}
      //check to see if we need to add the graph to the legend (prevents redundant entries
      //to legend)
      if(added[0] == 0 && !strcmp(code.c_str(),"B21"))
	{
	 
	  added[0] += 1;
	}
      if(added[1] == 0 && !strcmp(code.c_str(),"B22"))
	{
	  leg -> AddEntry(dummy2,"B22","p");
	  added[1] += 1;
	}
      if(added[2] == 0 && !strcmp(code.c_str(),"B23"))
	{
	  leg -> AddEntry(dummy2,"B23","p");
	  added[2] += 1;
	}
      if(added[3] == 0 && !strcmp(code.c_str(),"B24"))
	  {
	    leg -> AddEntry(dummy2,"B24","p");
	    added[3] += 1;
	  }
      */
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

/*
void makeCalFiles(char *filelist, int mode)
{
  ifstream tilelist;
  tilelist.open(filelist);
  int numOfFiles = countLines(filelist);
  //cout << numOfFiles << endl;
  for(int i = 0; i < numOfFiles; i++)
    {
      string name;
      cout << name << endl;
      tilelist >> name;
      makeUniTree(name,mode,1);
    }
}
*/
float getCorrFactor(int chan, int setup, int angle)
{
  
  
  float OTSfactor[9] = {1.51541,1.18528,0.604564,0.80545,0.798028,0.941173,0.669699,0.964993,1.51541};//old GSU test stand
  
  float NTSfactor[10] = {1,0.917691,0.90605,0.907409,0.878096,0.926225, 0.865125,0.931264,1.01907,1.01907};

  float NTSAvefactor[10] = {0.983826, 0.972284, 0.976437, 0.942392, 0.997507, 0.934308, 0.999448, 1.11689, 1.11689};

  float uniFactor[10][12] = {{0.989601, 0.957811, 0.844825, 0.894116, 0.987334, 0.911911, 1.09497, 0.92585, 0.883242, 1.07314, 1.02452, 0.87297},
			     {1.09243, 0.982417, 1.09423, 1.08497, 1.00973, 0.84541, 0.912819, 0.870002, 0.882522, 0.798823, 1.07152, 0.922318},
			     {1.0379, 0.987022, 1.09045, 1.03904, 1.00018, 1.01716, 0.962158, 1.07442, 1.0613, 1.05614, 0.96668, 1.06403},
			     {1.05944, 1.0591, 1.1273, 1.14251, 0.933805, 1.0133, 0.938138, 1.10554, 1.06226, 1.11945, 1.05903, 1.05031},
			     {0.903958, 0.899917, 0.911996, 0.930151, 0.906021, 0.904496, 0.816524, 0.932161, 0.890464, 0.925368, 0.862732, 0.928946},
			     {1.0384, 1.08864, 1.08739, 0.983534, 1.05889, 1.04756, 1.10376, 1.05079, 1.05043, 1.04497, 1.02763, 1.07738},
			     {1.00953, 1.05771, 1.05937, 1.0865, 1.04967, 1.0836, 1.05704, 1.05847, 1.14003, 0.940622, 1.00165, 0.856642},
			     {0.944529, 0.916359, 0.863106, 0.891162, 0.944426, 0.928753, 0.972719, 0.950543, 0.950041, 0.955648, 0.970133, 0.989863},
			     {0.9991, 1.0597, 1.03, 1.04918, 0.996916, 1.04336, 1.07335, 1.01073, 0.949444, 1.09776, 1.02661, 1.08458},
			     {0.9251, 0.991318, 0.891331, 0.898837, 1.11301, 1.20446, 1.06852, 1.02149, 1.13027, 0.988081, 0.989497, 1.15295}};//Uniplast test factor 

  float NTSUniFactor[10] = {0.959184,0.868481,0.908916,0.86014,0.833106,0.859274,0.869979,0.838039,0.9355,0.9355};//For redoing Uniplast tests with new test stand at GSU


  
  /*
  float dualFactor[8][12] = {{0.941543,0.84383,0.801166,1.07996,1,1,1,1,1,1,1,1},
			     {0.945356,0.889836,0.78879,1.07194,1,1,1,1,1,1,1,1},
			     {0.946926,0.846377,0.77764,1.06569,1,1,1,1,1,1,1,1},
			     {0.899934,0.854325,0.807202,1.08424,1,1,1,1,1,1,1,1},
			     {0.919826,0.780081,0.797401,1.05241,1,1,1,1,1,1,1,1},
			     {0.880365,0.927231,0.823714,1.049,1,1,1,1,1,1,1,1},
			     {0.887272,0.87827,0.770897,0.939147,1,1,1,1,1,1,1,1},
			     {0.912858,0.796032,0.911402,1.09011,1,1,1,1,1,1,1,1}};
  */
  /* float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,0.935977,1,0.854018,0.962745,1,1,1,1},
			     {0.847336,0.854614,0.806812,0.769812,0.934153,1,0.864736,0.972421,1,1,1,1},
			     {0.838685,0.821466,0.778553,0.70985,0.893511,1,0.826639,0.968322,1,1,1,1},
			     {0.88497,0.841338,0.788198,0.775766,0.947077,1,0.845317,0.993708,1,1,1,1},
			     {0.838685,0.872622,0.797924,0.831189,0.939296,1,0.921756,0.963874,1,1,1,1},
			     {0.865029,0.887264,0.816234,0.833545,0.97727,1,0.831858,0.985357,1,1,1,1},
			     {0.848827,0.860359,0.791134,0.811859,0.902238,1,0.793583,0.93354,1,1,1,1},
			     {0.831169,0.923422,0.877023,0.817548,0.99918,1,0.9256,0.979267,1,1,1,1}};*/

  float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,0.935977,0.918277,0.854018,0.962745,0.870416,0.967691,0.812866,0.873582},
			     {0.847336,0.854614,0.806812,0.769812,0.934153,0.946811,0.864736,0.972421,0.902587,0.88737,0.830939,0.888037},
			     {0.838685,0.821466,0.778553,0.70985,0.893511,0.938996,0.826639,0.968322,0.962354,0.955715,0.922084,0.967667},
			     {0.88497,0.841338,0.788198,0.775766,0.947077,0.938762,0.845317,0.993708,0.961574,1.04429,0.887333,0.966277},
			     {0.838685,0.872622,0.797924,0.831189,0.939296,0.935322,0.921756,0.963874,0.979116,0.82606,0.954908,0.875859},
			     {0.865029,0.887264,0.816234,0.833545,0.97727,0.969677,0.831858,0.985357,0.970586,1.00305,0.864868,0.8912},
			     {0.848827,0.860359,0.791134,0.811859,0.902238,0.900005,0.793583,0.93354,0.918294,0.926915,0.7502,0.934033},
			     {0.831169,0.923422,0.877023,0.817548,0.99918,1.01151,0.9256,0.979267,0.954242,1.00587,0.927643,0.99419}};
  
  
  if(setup==0) return 1/OTSfactor[chan];
  if(setup==1) return 1/NTSfactor[chan];
  if(setup==2) return 1/NTSAvefactor[chan];
  if(setup==3) return 1/uniFactor[chan][angle-1];
  if(setup==4) return 1/NTSUniFactor[chan];
  if(setup==5) return 1/dualFactor[chan-1][angle-1];
  //if(setup == 5) return 1; 

  return 0;
}

 

