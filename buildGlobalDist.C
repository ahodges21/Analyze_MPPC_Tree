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
const int colors[5] = {1,2,4,8,9};
int countLines(char *filelists);

void buildGlobalDist(int mode)
{

  TDatime *datemade = new TDatime();
  TFile *output = new TFile(Form("Master_Output_%d.root",datemade -> GetDate()),"RECREATE");

  gStyle -> SetOptStat(1110);
  TH1F *globalDist = new TH1F("globalDist","GlobalDist",40,0,2);
 
  if(mode == 0)
    {
      TH1F *tiles[12];
      ifstream tileSource[12];
      TCanvas *c[12];
      for(int i =0; i < 12; i++)
	{
	  tileSource[i].open(Form("excelLists/B%dexcel.txt",i+21));
	  tiles[i] = new TH1F(Form("B%d",i+21),Form("B%d",i+21),40,0,2);
	  tiles[i] -> SetTitle(Form("B%d;MPV_{Tile}/<MPV_{Refs}>;",i+21));
	  for(int j = 0; j < countLines(Form("excelLists/B%dexcel.txt",i+21)); j++)
	    {
	      string x;
	      tileSource[i] >> x;
	      cout <<Form("B%d: %s",i+21,x.c_str())<<endl;
	      tiles[i] -> Fill(stof(x));
	    }
	  globalDist -> Add(tiles[i],1);
	  c[i] = new TCanvas();
	  tiles[i] -> Draw();
	  output -> cd();
	  tiles[i] -> Write();
	  c[i] -> SaveAs(Form("excelLists/plots/B%d.pdf",i+21));
	}

 
  
      TCanvas *all = new TCanvas();
      globalDist -> SetTitle("");
      globalDist -> SetXTitle("MPV_{Tile}/<MPV_{Refs}>");
      globalDist -> Draw();
      globalDist -> Write();
      all -> SaveAs("excelLists/plots/global.pdf");
      output -> Close();
    }
  if(mode == 1)
    {
      ifstream globaldist;
      globaldist.open("excelLists/excelDist.txt");
      int nLines = countLines("excelLists/excelDist.txt");
      cout << nLines << endl;
      for(int i = 0; i < nLines; i++)
	{
	  string pr;
	  globaldist >> pr;
	  globalDist -> Fill(stof(pr));
	}
      TCanvas *all = new TCanvas();
      globalDist -> SetTitle("");
      globalDist -> SetXTitle("MPV_{Tile}/<MPV_{Refs}>");
      globalDist -> Draw();
      globalDist -> Write();
      all -> SaveAs("excelLists/plots/global.pdf");
      output -> Close();
    }
}

void ShipmentComp(int angle, const int nShips)
{
  
  gStyle -> SetOptStat(0);
  int color = 0;
  ifstream shipsIn[nShips];
  TH1F *hShips[nShips];
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  for(int i = 0; i < nShips; i++)
    {
      shipsIn[i].open(Form("excelLists/shipBreakdown/B%d/B%d_%d.txt",angle,angle,i+1));
      hShips[i] = new TH1F(Form("Shipment_%d",i),Form("Shipment_%d",i),40,0,2);
      for(int j = 0; j < countLines(Form("excelLists/shipBreakdown/B%d/B%d_%d.txt",angle,angle,i+1));j++)
	{
	  float x;
	  shipsIn[i] >> x;
	  //cout << x << endl;
	  hShips[i] -> Fill(x);
	}
      
      hShips[i] -> SetTitle(Form("B%d;MPV_{Tile}/<MPV_{Refs}>;",angle));
      hShips[i] -> SetLineColor(colors[color]);
      if(hShips[i]->GetEntries() != 0)leg -> AddEntry(hShips[i],Form("Shipment %d",i+1),"l");
      if(i == 0)
	{
	  hShips[i] -> GetYaxis() -> SetRangeUser(0,20);
	  hShips[i] ->Draw("");
	}
      else
	{
	  if(hShips[i] -> GetEntries() !=0)hShips[i] -> Draw("same");
	}
      leg -> Draw("same");
      color++;
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
