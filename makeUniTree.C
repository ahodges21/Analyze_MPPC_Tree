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
const int Trig1 = 10;//10; //0 for uniplast data
const int Trig2 = 11;//11; //1 for uniplast data
const int thresh = 250;//250 for GSU, 475 for Uniplast
const int setup = 5;
const int strl = 72;
const int nBins = 100;
TH1F* makeUniTree(string f, int mode, int iscalib);
void makeRunningDists(char *filelist, int mode);
float extractPerfRat(char* filelist, int mode);
float extractSBR(TH1F *histin);
int countLines(char *filelist);
float getCorrFactor(int chan, int setup, int angle);
//Mode 1 makes a distribution stack MPV's from the file list provided
//Mode 2 makes a graph that plots the average stack MPV versus the test
//number to track the performance over time. 

TH1F* makeUniTree(string f, int mode = 0, int iscalib = 0)
{
  TFile *fin = new TFile(f.c_str());
  TTree *mppc = (TTree*)fin->Get("mppc");
  TH1F *mpvDist = new TH1F("mpvDist","mpvDist",200,400,1400);
  UShort_t chg[32];
  mppc -> SetBranchAddress("chg",&chg);
  TH1F *hcalNoTrig[nChans];
  TH1F *hcalSoftTrig[nChans];
  string angle = f.substr(strl,2);
 
  stringstream convert(angle);
  int angleco = 0; convert >> angleco;
 
  angleco -= 20;
 
  
    

  int nEvents = mppc -> GetEntries();

  //int chanList[nChans] = {0,1,2,3,8,9,10,11,12,13}; //Uniplast Channel List

  //int chanList[nChans] = {0,1,2,3,4,5,6,7}; // GSU Channel List

  //int chanList[nChans] = {2,3,4,5,6,7};

  //int chanList[nChans] = {0,1,2,3,8,9}; //Uniplast list without inner tiles from pilot tests

  //int chanList[nChans] = {0,1,2,3,4,5,6,7,8}; //GSU test stand for first batch of pre-production tiles

  int chanList[nChans] = {11,31,30,29,28,23,22,21,20,10};
  
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
    }
  for(int i = start; i < end ;i++)
    {
      
      
      float performance = 0;
      if(mode == 2) performance = (Fits[i] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(i,setup,angleco);
      if(mode == 3) performance = (Fits[i] -> GetParameter(1)/mpvAve)*getCorrFactor(i,setup,angleco);
      if(mode == 4) performance = (Fits[i] -> GetParameter(1)/(0.5*(Fits[0] -> GetParameter(1) + Fits[nChans-1] -> GetParameter(1))))*getCorrFactor(i,setup,angleco);
      
	
     
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
      TFile *output = new TFile(Form("calibFiles/%sCal.root",f.substr(f.length()-12,5).c_str()),"RECREATE");
      //TFile *output = new TFile(Form("B21_%dcal.root",rand()%100),"RECREATE");
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
  
  //if(mode == 2) return mpvAve;
  //cout << "Average: " << mpvAve << endl;
  //cout << "Upper Bound: " << mpvAve*1.1 << endl;
  //cout << "Lower Bount: " << mpvAve*0.9 << endl;
  /*for(int i = 0; i < nChans; i++)
    {
      if(Fits[i] -> GetParameter(1) > mpvAve * 1.1 || Fits[i] -> GetParameter(1) < mpvAve*0.9)
	   {
	      hcalSoftTrig[i]->GetXaxis()->SetTitle("FAILED: Bad performance");
	    hcalSoftTrig[i]->GetXaxis()->CenterTitle();
	    hcalSoftTrig[i]->GetXaxis()->SetTitleSize(0.10);
	    hcalSoftTrig[i]->GetXaxis()->SetTitleOffset(-1.50);
	    hcalSoftTrig[i]->GetYaxis()->SetTitle("Failed");
	    hcalSoftTrig[i]->GetYaxis()->SetTitleSize(0.06);
	    hcalSoftTrig[i]->SetLabelColor(2,"X");
	    hcalSoftTrig[i]->SetLabelColor(2,"Y");
	    hcalSoftTrig[i]->SetAxisColor(2,"X");
	    hcalSoftTrig[i]->SetAxisColor(2,"Y");
	   }
	   }*/



  return 0;
}

float extractPerfRat(char* filelist, int mode)
{
  
 
 
  int numOfFiles = countLines(filelist);
  float minPerf = 4;
  int minPerfChan = 0;
  string minPerfFile = ""; 
  fstream link; link.open(filelist);
  
  for(int l = 0; l < numOfFiles; l++)
    {
      
      string f;
      link >> f;
      string angle = f.substr(71,2);
      
      stringstream convert(angle);
      int angleco = 0; convert >> angleco;
      
      angleco -= 20;
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
	      performance = (Fits[k] -> GetParameter(1)/Fits[0]->GetParameter(1))*getCorrFactor(k,5,angleco);
	    }
	  else if(mode == 3)
	    {
	      performance = (Fits[k] -> GetParameter(1)/mpvAve)*getCorrFactor(k,5,angleco);
	    }
	  if(abs(1-performance) < minPerf)
	    {
	      minPerfChan = k;
	      minPerfFile = f;
	      minPerf = abs(1 - performance);
      
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
  cout << "Ratio: " << minPerf << endl;
  return 0;
}
  



void makeRunningDists(char *filelist, int mode1, int mode2, char* outname, int save = 0)
{
  
  ifstream list;
  int numOfFiles = countLines(filelist);
  cout << numOfFiles << endl;
  list.open(filelist);
  TH1F *runningDist;
  TFile *output = new TFile(Form("%s.root",outname), "RECREATE");
  if(mode1 == 1){
    if(mode2 == 1)
      {
	runningDist = new TH1F("runningDist","runningDist",200,400,1400);
      }
    else
      {
	runningDist = new TH1F("runningDist","runningDist",nBins,0,2);
      }
    for(int i = 0; i < numOfFiles; i++)
      {
	string name;
	list >> name;
	//cout << name << endl;
	runningDist -> Add(makeUniTree(name,mode2,0));
    }
    runningDist -> Draw();
    //runningDist -> Fit("gaus","","");
    runningDist -> SetName(outname);
  }
  output -> cd();
  if(save == 1)runningDist -> Write();
  output -> Close();
  /* 
  if(mode1 == 2){
    float mpv[numOfFiles];
    float x[numOfFiles];
    for(int i = 0; i < numOfFiles; i++)
      {
	string name;
	list >> name;
	x[i] = i+1;
	mpv[i] = extractAve(name);
      }
    TGraph *Track = new TGraph((int)sizeof(mpv)/(sizeof(mpv[0])),x, mpv);
    Track -> SetMarkerStyle(4);
  }
  */

 
  
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
void superImpose(int mode = 2, char* globalname = "", char* bd = "")
{
  gStyle -> SetOptStat(1101);
  TH1F *hists[numSec];
  TFile *secs[numSec];
  TFile *global = new TFile(Form("%s.root",globalname));
  TH1F *allSecs = (TH1F*)global->Get(globalname);
  allSecs -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
  TCanvas *globalc = new TCanvas();
  allSecs -> SetTitle("");
  allSecs -> SetName("All Tiles");
  allSecs -> Draw();
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  //leg -> AddEntry(allSecs,"All Tiles","l");
  TCanvas *breakdown = new TCanvas();
  for(int i = 0; i < numSec; i++)
    {
      secs[i] = new TFile(Form("B%d%s.root",i+21,bd));
      hists[i] = (TH1F*)secs[i] -> Get(Form("B%d%s",i+21,bd));
      //hists[i] -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Test}>");
      hists[i] -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
      hists[i] -> SetName(Form("B%d",i+21));
      hists[i] -> SetLineColor(i+1);
      if(i == 3)hists[i] -> SetLineColor(i+3);
      hists[i] -> SetMarkerStyle(4);
      hists[i] -> SetTitle("");
      if(mode == 0){
      if(i == 0)hists[i] -> Draw();
      if(i!=0)hists[i]->Draw("same");
      }
      leg -> AddEntry(hists[i],Form("B%d",i+21),"l");
    }
  leg -> Draw("same");
  if(mode == 2){
    TCanvas *c1 = new TCanvas();
    c1 -> Divide(2,2);
    for(int i = 0; i < numSec; i++)
      {
	c1 -> cd(i+1);
	hists[i] -> GetXaxis() -> SetTitle("MPV_{Tile}/<MPV_{Refs}>");
	hists[i] -> GetXaxis() -> SetRangeUser(0,2);
	
	hists[i] ->Draw();
	
      }
  }
}

const int fileNum = 22;
void makePositDep(char *filelist, int setup)
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
      //cout << name << endl;
      ins[i] = new TFile(name.c_str());
      TGraph *dummy = (TGraph*)ins[i] -> Get("positionDep");
      TGraph *dummy2 = new TGraph();
      dummy2 -> SetMarkerStyle(4);
      string code = name.substr(0,3);
      //cout << code << endl;
      string angle = code.substr(1,2);
      stringstream convert(angle);
      int angleco =0; convert >> angleco;
      angleco -= 20;
      cout << angleco << endl;
      for(int j = 0; j < nChans; j++)
	{
	  if(j != 0 && j!=9){
	    double x,y;
	    dummy -> GetPoint(j,x,y);
	  
	    dummy2 -> SetPoint(j-1,x,y*getCorrFactor(j,setup,angleco));
	    /*
	      else{
	      dummy -> SetPoint(j+1,x,y*getCorrFactor(j,setup));
	      }
	    */
	    corrFac[j] += y*getCorrFactor(j,setup,angleco);
	    }
	    
	}
      
      
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
	  leg -> AddEntry(dummy2,"B21","p");
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
  leg -> Draw("same");

 
}
  
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

float getCorrFactor(int chan, int setup, int angle)
{
  
  
  float OTSfactor[9] = {1.51541,1.18528,0.604564,0.80545,0.798028,0.941173,0.669699,0.964993,1.51541};//old GSU test stand
  
  float NTSfactor[10] = {1,0.917691,0.90605,0.907409,0.878096,0.926225, 0.865125,0.931264,1.01907,1.01907};

  float NTSAvefactor[10] = {0.983826, 0.972284, 0.976437, 0.942392, 0.997507, 0.934308, 0.999448, 1.11689, 1.11689};

  float uniFactor[10] = {1.06388, 1.03855, 1.09293, 0.910196, 1.05224, 1.04943, 0.906457, 1.03199, 0.92796, 0.92796};

  float NTSUniFactor[10] = {0.959184,0.868481,0.908916,0.86014,0.833106,0.859274,0.869979,0.838039,0.9355,0.9355};
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
  float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,1,1,1,1,1,1,1,1},
			     {0.847336,0.854614,0.806812,0.769812,1,1,1,1,1,1,1,1},
			     {0.838685,0.821466,0.778553,0.70985,1,1,1,1,1,1,1,1},
			     {0.88497,0.841338,0.788198,0.775766,1,1,1,1,1,1,1,1},
			     {0.838685,0.872622,0.797924,0.831189,1,1,1,1,1,1,1,1},
			     {0.865029,0.887264,0.816234,0.833545,1,1,1,1,1,1,1,1},
			     {0.848827,0.860359,0.791134,0.811859,1,1,1,1,1,1,1,1},
			     {0.831169,0.923422,0.877023,0.817548,1,1,1,1,1,1,1,1}};
  
  if(setup==0) return 1/OTSfactor[chan];
  if(setup==1) return 1/NTSfactor[chan];
  if(setup==2) return 1/NTSAvefactor[chan];
  if(setup==3) return 1/uniFactor[chan];
  if(setup==4) return 1/NTSUniFactor[chan];
  if(setup==5) return 1;//1/dualFactor[chan-1][angle-1];
  //if(setup == 5) return 1; 

  return 0;
}

 

