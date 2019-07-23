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

void makeFactorMean()
{
  float dualFactor[8][12] = {{0.810873,0.814866,0.794136,0.763176,0.935977,0.918277,0.854018,0.962745,0.870416,0.967691,0.812866,0.873582},
			     {0.847336,0.854614,0.806812,0.769812,0.934153,0.946811,0.864736,0.972421,0.902587,0.88737,0.830939,0.888037},
			     {0.838685,0.821466,0.778553,0.70985,0.893511,0.938996,0.826639,0.968322,0.962354,0.955715,0.922084,0.967667},
			     {0.88497,0.841338,0.788198,0.775766,0.947077,0.938762,0.845317,0.993708,0.961574,1.04429,0.887333,0.966277},
			     {0.838685,0.872622,0.797924,0.831189,0.939296,0.935322,0.921756,0.963874,0.979116,0.82606,0.954908,0.875859},
			     {0.865029,0.887264,0.816234,0.833545,0.97727,0.969677,0.831858,0.985357,0.970586,1.00305,0.864868,0.8912},
			     {0.848827,0.860359,0.791134,0.811859,0.902238,0.900005,0.793583,0.93354,0.918294,0.926915,0.7502,0.934033},
			     {0.831169,0.923422,0.877023,0.817548,0.99918,1.01151,0.9256,0.979267,0.954242,1.00587,0.927643,0.99419}};
  float avg[12];
  float avgerr[12];
  for(int i = 0; i < 12; i++)
    {
      float average = 0;
      for(int j = 0; j < 8; j++)
	{
	  average += dualFactor[j][i];
	}
      avg[i] = average/8;
    }

  for(int i = 0; i < 12; i++)
    {
      float std = 0;
      for(int j = 0; j < 8; j++)
	{
	  std += pow(avg[i] - dualFactor[j][i],2);
	}
      avgerr[i] = sqrt(std/8.);
    }

  TCanvas *c1 = new TCanvas();
  float tiles[12] = {21,22,23,24,25,26,27,28,29,30,31,32};
  TGraphErrors *corrFactors = new TGraphErrors(12,tiles,avg,0,avgerr);
  corrFactors -> SetMarkerStyle(4);
  corrFactors -> SetTitle("");
  corrFactors -> GetXaxis() -> SetTitle("Tile Angle");
  corrFactors -> GetYaxis() -> SetTitle("<#alpha_{m,n}>");
  corrFactors -> Draw("ap");

  float percentErr[12];
  for(int i = 0; i< 12; i++)
    {
      percentErr[i] = avgerr[i];
    }
  TCanvas *c2 = new TCanvas();
  TGraph *perErr = new TGraph(12,tiles,percentErr);
  perErr -> SetTitle("");
  perErr -> GetXaxis() -> SetTitle("Tile Angle");
  perErr -> GetYaxis() -> SetTitle("<#alpha_{m,n}> % Err");
  perErr -> GetYaxis() -> SetRangeUser(0,.10);
  perErr -> SetMarkerStyle(4);
  perErr -> Draw("ap");
}
	  
