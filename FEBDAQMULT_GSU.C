// Mainframe macro generated from application: /home/home/root-6.02.02/bin/root.exe
// By ROOT version 6.02/02 on 2015-05-17 23:03:49
//
// History: July 11, 2017, hexc
// 

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif

#include "Riostream.h"
#include "TMath.h"
#include "TF1.h"
#include "TTree.h"
#include "time.h"
#include <sys/timeb.h>

#define maxpe 10 //max number of photoelectrons to use in the fit
int NEVDISP=200; //number of lines in the waterfall event display
const Double_t initpar0[7]={7000,100,700,9.6,1.18,0.3,0.5};
const Double_t initpar1[7]={3470,100,700,9.5,2.25,3e-3,3.7e-2};
Double_t peaks[maxpe]; //positions of peaks in ADC counts
Double_t peaksint[maxpe]; //expected integral of each p.e. peak
UShort_t VCXO_Value=500; //DAC settings to correct onboard VCTCXO
UShort_t VCXO_Values[256]; //DAC settings to correct onboard VCTCXO per board, index=mac5
TGNumberEntry *fNumberEntry75;
TGNumberEntry *fNumberEntry755;
TGNumberEntry *fNumberEntry886;
TGNumberEntry *fNumberEntry8869;
TGNumberEntry *fNumberEntryTME;
int hvoffcount = 0;
//TGLabel *fLabel;
TGStatusBar *fStatusBar739;
   TGRadioButton * fChanProbe[33];
   TGCheckButton * fChanEnaAmp[34];
   TGCheckButton * fChanEnaTrig[33];
   TGNumberEntry * fChanGain[32];
   TGNumberEntry * fChanBias[32];
   TGCheckButton *fUpdateHisto;
   TGCheckButton *fUpdateVCXO;
   TGTab *fTab683;
   TGLabel *fLabel7;
   TBenchmark *BenchMark; 
TF1* f0;
TF1* f1;

UChar_t bufPMR[1500];
UChar_t bufSCR[1500];
UChar_t buf[1500];
TH1F * hst[32];
TCanvas *c=0;
TCanvas *c1=0;
TCanvas *c3=0;
TCanvas *c4=0;
TCanvas *c5=0;
TCanvas *c6=0;
TGraph *grevrate=0;
TGraph *gr=0;
TGraph *gts0[256];
TGraph *gts1[256];
TH1F *hcprof=0;
TH2F *hevdisp=0;
TGTextButton *bHVStatus;
void FillHistos(int truncat);
  int evs=0; //overall events per DAQ
 // int evs_notfirst=0; //overall events per DAQ without very first request
  int evsperrequest=0;
FEBDTP* t;
Int_t chan=0; //channel to display on separate canvas c1
Int_t BoardToMon=0; //board to display on separate canvas c1
TTree * tr;
TTree * tc;
int RunOn=0;

UChar_t mac5=0x00;   // moved this line here by hexc 7/11/2017

time_t tm0,tm1;

uint32_t CHAN_MASK=0xFFFFFFFF; // by default all channels are recorded

void FEBGUI();
void SetThresholdDAC1(UShort_t dac1);
UShort_t GetThresholdDAC1();
void SetThresholdDAC2(UShort_t dac2);
int Init(const char* iface);
float GetTriggerRate();
void UpdateConfig();
void UpdateHisto();
void UpdateBoardMonitor();
void DAQ(int nev);
void RescanNet();
UChar_t ConfigGetGain(int chan);
UChar_t ConfigGetBias(int chan);
void ConfigSetGain(int chan, UChar_t val);
void ConfigSetBias(int chan, UChar_t val);
void ConfigSetFIL(uint32_t mask1, uint32_t mask2, uint8_t majority); 
void HVOF();
void HVON();

//Adding thermocouple software, original framework done by John Haggerty
//-Anthony Hodges
UInt_t tnow = 0, tlast = 0;

struct therm {
  Double_t utime;
  Double_t temperature[8];
};

therm oneread;

therm thermocouples()
{
  TString the_output=gSystem->GetFromPipe("/home/hexc/Software/BNL_Kistenev/FEBDAQMULT/thermoBuild/usb-thermocouples");

  TObjArray *tx = the_output.Tokenize(","); 
  
  tx->Print();

  therm oneread;

  TString xx = ((TObjString *)(tx->At(0)))->String();
  oneread.utime = xx.Atof();
  Double_t temperature[8];
  for (Int_t i = 1; i < tx->GetEntries(); i++) {
    xx = ((TObjString *)(tx->At(i)))->String();
    oneread.temperature[i-1] = xx.Atof();
  }

  return oneread;
}


UInt_t GrayToBin(UInt_t n)
{
UInt_t res=0;
int a[32],b[32],i=0,c=0;

for(i=0; i<32; i++){
if((n & 0x80000000)>0) a[i]=1;
else a[i]=0;
n=n<<1;
//printf("%1d",a[i]);
}

b[0]=a[0];
//printf("  %1d",b[0]);
for(i=1; i<32; i++){
if(a[i]>0) if(b[i-1]==0) b[i]=1; else b[i]=0;
else b[i]=b[i-1];
//printf("%1d",b[i]);
}

for(i=0; i<32; i++){
res=(res<<1);
res=(res | b[i]); 
}
//printf("\n");

  return res;
}


Double_t mppc0( Double_t *xx, Double_t *par)
{
  Double_t retval=0; //return value 
  Double_t N=par[0]; //normalisation factor
  Double_t gain=par[1]; //adc counts per p.e.
  Double_t zero=par[2]; //position of the pedestal
  Double_t noise=par[3]; //RMS of the electronic noise
  Double_t avnpe=par[4]; //mean number of photoelectrons
  Double_t exess=par[5]; //exess poisson widening factor
  Double_t xtalk=par[6]; //x-talk factor
  Double_t x=xx[0]; //argument  in ADC counts
  
  for(int i=0; i<=maxpe; i++)
  {
    peaks[i]=zero+gain*i;
    peaksint[i]=TMath::Poisson(i,avnpe);
    if(i>=2) peaksint[i]=peaksint[i]+peaksint[i-1]*xtalk;
  }
   for(int i=0; i<=maxpe; i++)
  {
    retval+=peaksint[i]*(TMath::Gaus(x,peaks[i],sqrt(noise*noise+i*exess), kTRUE));
  //  retval+=TMath::Gaus(x,peaks[i],noise, kTRUE);
  } 
  retval=retval*N;
  return retval;
}

Double_t mppc1( Double_t *xx, Double_t *par) // from http://zeus.phys.uconn.edu/wiki/index.php?title=Characterizing_SiPMs
{
  Double_t retval=0; //return value 
  Double_t N=par[0]; //normalisation factor
  Double_t gain=par[1]; //adc counts per p.e.
  Double_t zero=par[2]; //position of the pedestal
  Double_t noise=par[3]/gain; //normalized RMS of the electronic noise
  Double_t avnpe=par[4]; //mean number of photoelectrons
  Double_t exess=par[5]/gain; //normalized exess poisson widening factor
  Double_t mu=par[6]; //x-talk factor
  Double_t q=(xx[0]-zero)/gain; //argument  in ADC counts normalized and zeroized
  
  for(int p=0; p<=maxpe; p++) for(int s=0; s<=maxpe; s++)
  {
    retval+= TMath::Poisson(p,avnpe) * TMath::Poisson(s,avnpe*mu) * TMath::Gaus(q,p+s,sqrt(noise*noise+exess*exess*(p+q)), kTRUE);
  }
    retval=retval*N;
  return retval;
}



void FEBDAQMULT_GSU(const char *iface="eth1")
{
  if(Init(iface)==0) return;
  FEBGUI();
 UpdateConfig();
 fNumberEntry8869->SetLimitValues(0,t->nclients-1);
 for(int feb=0; feb<t->nclients; feb++)  VCXO_Values[feb]=VCXO_Value;
 UpdateBoardMonitor(); 
}

void ConfigSetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index, Bool_t value)
{
  UChar_t byte;
  UChar_t mask;
  byte=buffer[(bitlen-1-bit_index)/8];
  mask= 1 << (7-bit_index%8);
  byte=byte & (~mask);
  if(value) byte=byte | mask;
  buffer[(bitlen-1-bit_index)/8]=byte;
}

bool ConfigGetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index)
{
  UChar_t byte;
  UChar_t mask;
  byte=buffer[(bitlen-1-bit_index)/8];
  mask= 1 << (7-bit_index%8);
  byte=byte & mask;
  if(byte!=0) return true; else return false; 
}

void SetDstMacByIndex(int i)
{
  if(i>=t->nclients || i<0) return;
  for(int j=0;j<6;j++) t->dstmac[j]=t->macs[i][j];
  mac5=t->macs[i][5]; 
}


void UpdateConfig()
{
char bsname[32];
 uint8_t bufFIL[256]; 

//t->ReadBitStream("CITIROC_SCbitstream_TESTS.txt",bufSCR);
t->ReadBitStream("CITIROC_PROBEbitstream.txt",bufPMR);
for(int feb=0; feb<t->nclients; feb++)
{
SetDstMacByIndex(feb);
//t->dstmac[5]=0xff; //Broadcast
sprintf(bsname,"CITIROC_SC_SN%03d.txt",t->dstmac[5]);
//if(!(t->ReadBitStream(bsname,bufSCR))) t->ReadBitStream("CITIROC_SC_DEFAULT.txt",bufSCR);
if(!(t->ReadBitStream(bsname,bufSCR))) t->ReadBitStream("CITIROC_SC_PROFILE1.txt",bufSCR);


  *((uint32_t*)(&(bufFIL[0])))=*((uint32_t*)(&(bufSCR[265]))); //copy trigger enable channels from SCR to FIL tregister


t->SendCMD(t->dstmac,FEB_SET_RECV,fNumberEntry75->GetNumber(),t->srcmac);
t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
t->SendCMD(t->dstmac,FEB_WR_PMR,0x0000,bufPMR);
t->SendCMD(t->dstmac,FEB_WR_FIL,0x0000,bufFIL);

if(feb==BoardToMon)  {
  for(int i=265; i<265+32;i++)
  if(ConfigGetBit(bufSCR,1144,i)) fChanEnaTrig[i-265]->SetOn(); else fChanEnaTrig[i-265]->SetOn(kFALSE);
  if(ConfigGetBit(bufSCR,1144,1139)) fChanEnaTrig[32]->SetOn(); else fChanEnaTrig[32]->SetOn(kFALSE);
  for(int i=0; i<32;i++)
  if(ConfigGetBit(bufSCR,1144,633+i*15)) fChanEnaAmp[i]->SetOn(kFALSE); else fChanEnaAmp[i]->SetOn();
  fChanEnaAmp[33]->SetOn(kFALSE);fChanEnaAmp[32]->SetOn(kFALSE);
  for(int i=0; i<32;i++)
  if(ConfigGetBit(bufPMR,224,96+i)) fChanProbe[i]->SetOn(); else fChanProbe[i]->SetOn(kFALSE);
  fNumberEntry755->SetNumber(GetThresholdDAC1());
  for(int i=0; i<32;i++) fChanGain[i]->SetNumber(ConfigGetGain(i));
  for(int i=0; i<32;i++) fChanBias[i]->SetNumber(ConfigGetBias(i));

      }
}
//fNumberEntry755->SetNumber();
}

UChar_t ConfigGetGain(int chan)
{
 UChar_t val=0;
 for(int b=0;b<6;b++)
 {
   val=val << 1;
   if(ConfigGetBit(bufSCR,1144,619+chan*15+b)) val=val+1;
 } 
 return val;
}

UChar_t ConfigGetBias(int chan)
{
 UChar_t val=0;
 for(int b=0;b<8;b++)
 {
   val=val << 1;
   if(ConfigGetBit(bufSCR,1144,331+chan*9+b)) val=val+1;
 } 
 return val;
}

void ConfigSetGain(int chan, UChar_t val)
{
  UChar_t mask=1<<5;
 for(int b=0;b<6;b++)
 {
   if((val & mask)>0) ConfigSetBit(bufSCR,1144,619+chan*15+b,kTRUE); else ConfigSetBit(bufSCR,1144,619+chan*15+b,kFALSE);
   mask=mask>>1;
 } 
  
}

void ConfigSetBias(int chan, UChar_t val)
{
  UChar_t mask=1<<7;
 for(int b=0;b<8;b++)
 {
   if((val & mask)>0) ConfigSetBit(bufSCR,1144,331+chan*9+b,kTRUE); else ConfigSetBit(bufSCR,1144,331+chan*9+b,kFALSE);
   mask=mask>>1;
 } 

}



void PrintConfig(UChar_t *buffer, UShort_t bitlen)
{
    UChar_t byte;
  UChar_t mask;
  for(int i=0; i<bitlen;i++)
 {
  byte=buffer[(bitlen-1-i)/8];
  mask= 1 << (7-i%8);
  byte=byte & mask;
  if(byte==0) printf("0"); else printf("1");
 }
 printf("\n"); 
} 

void SendConfig()
{
  uint32_t trigmask=0;
 uint8_t bufFIL[256]; 

  for(int i=265; i<265+32;i++)
  if(fChanEnaTrig[i-265]->IsOn()) ConfigSetBit(bufSCR,1144,i,1); else  ConfigSetBit(bufSCR,1144,i,0);

  if(fChanEnaTrig[32]->IsOn()) ConfigSetBit(bufSCR,1144,1139,1); else  ConfigSetBit(bufSCR,1144,1139,0); //OR32 enable

  for(int i=0; i<32;i++)
  if(fChanEnaAmp[i]->IsOn()) ConfigSetBit(bufSCR,1144,633+i*15,0); else  ConfigSetBit(bufSCR,1144,633+i*15,1);

  for(int i=0; i<32;i++)
  if(fChanProbe[i]->IsOn()) ConfigSetBit(bufPMR,224,96+i,1); else  ConfigSetBit(bufPMR,224,96+i,0);

  for(int i=0; i<32;i++) ConfigSetBias(i, fChanBias[i]->GetNumber());
  for(int i=0; i<32;i++) ConfigSetGain(i, fChanGain[i]->GetNumber());
  
  for(int i=0; i<32;i++)
  if(fChanEnaTrig[i]->IsOn()) trigmask = trigmask | (0x1 << i);
  *((uint32_t*)(&(bufFIL[0])))=trigmask;

//for(int feb=0; feb<t->nclients; feb++)
// {
//  SetDstMacByIndex(feb);  
t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
  t->SendCMD(t->dstmac,FEB_WR_PMR,0x0000,bufPMR);
  t->SendCMD(t->dstmac,FEB_WR_FIL,0x0000,bufFIL);
// }
}

void SendAllChecked()
{
  for(int i=0; i<32;i++)
  {
     fChanEnaAmp[i]->SetOn(); 
     ConfigSetBit(bufSCR,1144,633+i*15,0); 
  }

  SetDstMacByIndex(BoardToMon);  
//t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
//  t->SendCMD(t->dstmac,FEB_WR_PMR,0x0000,bufPMR);
// }
  fChanEnaAmp[33]->SetOn(kFALSE);fChanEnaAmp[32]->SetOn(kFALSE);
}
void SendAllUnChecked()
{
  for(int i=0; i<32;i++)
  {
     fChanEnaAmp[i]->SetOn(kFALSE); 
     ConfigSetBit(bufSCR,1144,633+i*15,1); 
  }

  SetDstMacByIndex(BoardToMon);  
//t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
//  t->SendCMD(t->dstmac,FEB_WR_PMR,0x0000,bufPMR);
// }
  fChanEnaAmp[33]->SetOn(kFALSE);fChanEnaAmp[32]->SetOn(kFALSE);
}

UShort_t chg[32];
UInt_t ts0,ts1;
UInt_t ts0_ref, ts1_ref;
UInt_t ts0_ref_MEM[256], ts1_ref_MEM[256]; //memorized time stamps for all febs
long int ts0_ref_AVE[256]; //Average value for PPS ts0, used for VCXO feedback
Int_t ts0_ref_IND[256]; //Number of averaged values for PPS ts0, used for VCXO feedback
Bool_t NOts0=false,NOts1=false;
UInt_t ts0_ref_mon, ts1_ref_mon;
Bool_t NOts0_mon=false,NOts1_mon=false;

void RescanNet()
{
 t->nclients=0;
 if(t->ScanClients()==0) {printf("No clients connected, exiting\n"); return;}; 
 t->PrintMacTable();
 t->VCXO=500;
 for(int feb=0; feb<t->nclients; feb++)  { ts0_ref_AVE[t->macs[feb][5]]=0;ts0_ref_IND[t->macs[feb][5]]=0; }
 UpdateConfig();
 fNumberEntry8869->SetLimitValues(0,t->nclients-1);
 for(int feb=0; feb<t->nclients; feb++)  VCXO_Values[feb]=VCXO_Value;
 UpdateBoardMonitor(); 
 
}


int Init(const char *iface="eth1")
{
  t=new FEBDTP(iface);
 if(t->ScanClients()==0) {printf("No clients connected, exiting\n"); return 0;};
 t->PrintMacTable(); 
 
 t->VCXO=500;
 for(int feb=0; feb<t->nclients; feb++)  { ts0_ref_AVE[t->macs[feb][5]]=0;ts0_ref_IND[t->macs[feb][5]]=0; }

 t->setPacketHandler(&FillHistos);
   char str1[32];
  char str2[32];
  SetDstMacByIndex(0);
  evs=0;
  for(int i=0;i<32;i++) {
      sprintf(str1,"h%d",i);
      sprintf(str2,"ADC # %d",i);
      hst[i]=new TH1F(str1,str2,820,0,4100);
      hst[i]->GetXaxis()->SetTitle("ADC value");
      hst[i]->GetYaxis()->SetTitle("Events");

    }
  for(int i=0;i<256;i++) gts0[i]=new TGraph();
  for(int i=0;i<256;i++) gts1[i]=new TGraph();
  hcprof=new TH1F("hcprof","Channel profile",32,0,32);
  hcprof->GetXaxis()->SetTitle("Channel number");
  hcprof->GetYaxis()->SetTitle("Integrated amplitude, ADC");
  hevdisp=new TH2F("hevdisp","Event Display",32,0,32,NEVDISP,0,NEVDISP);
  hevdisp->GetXaxis()->SetTitle("Channel number");
  hevdisp->GetYaxis()->SetTitle("Event number");
  grevrate= new TGraph();
  f0=new TF1("mppc0",mppc0,0,2000,7);
  f0->SetParameters(initpar0);
  f0->SetNpx(1000);
  f0->SetParNames("Norm","Gain","Pedestal","NoiseRMS","Npe","exess","X-talk");
  f1=new TF1("mppc1",mppc1,0,2000,7);
  f1->SetParameters(initpar1);
  f1->SetNpx(1000);
  f1->SetParNames("Norm","Gain","Pedestal","NoiseRMS","Npe","exess","X-talk");
  f1->SetLineColor(kBlue);
  tr=new TTree("mppc","mppc");
  tr->Branch("mac5",&mac5,"mac5/b");
  tr->Branch("chg",chg,"chg[32]/s");
  tr->Branch("ts0",&ts0,"ts0/i");
  tr->Branch("ts1",&ts1,"ts1/i");
  tr->Branch("ts0_ref",&ts0_ref,"ts0_ref/i");
  tr->Branch("ts1_ref",&ts1_ref,"ts1_ref/i");
  
  //Adding thermocouple TTree, Anthony Hodges
  tc = new TTree("tc","OM-USB-THERM");
  tc->Branch("utime",&oneread.utime,"utime/D");
  tc->Branch("temperature",oneread.temperature,"temperature[8]/D");


   BenchMark=new TBenchmark();
   BenchMark->Start("Poll");
return 1;
}

UInt_t overwritten=0;
UInt_t lostinfpga=0;
UInt_t total_lost=0;

void FillHistos(int truncat)  // hook called by libFEBDTP when event is received
{
  int jj;
  int kk;
//UInt_t T0,T1;
UShort_t adc;
int evspack=0;
UInt_t tt0,tt1;
UChar_t ls2b0,ls2b1; //least sig 2 bits

        jj=0;
        while(jj<truncat-18)
         {
        //printf(" Remaining events: %d\n",(t->gpkt).REG);
        //printf("Flags: 0x%08x ",*(UInt_t*)(&(t->gpkt).Data[jj]));
        overwritten=*(UShort_t*)(&(t->gpkt).Data[jj]); 
	jj=jj+2;
        lostinfpga=*(UShort_t*)(&(t->gpkt).Data[jj]); 
	jj=jj+2;
        ts0=*(UInt_t*)(&(t->gpkt).Data[jj]); jj=jj+4; 
        ts1=*(UInt_t*)(&(t->gpkt).Data[jj]); jj=jj+4; 
//	printf("T0=%u ns, T1=%u ns \n",ts0,ts1);
        ls2b0=ts0 & 0x00000003;
        ls2b1=ts1 & 0x00000003;
        tt0=(ts0 & 0x3fffffff) >>2;
        tt1=(ts1 & 0x3fffffff) >>2;
        tt0=(GrayToBin(tt0) << 2) | ls2b0;
        tt1=(GrayToBin(tt1) << 2) | ls2b1;
        tt0=tt0+5;//IK: correction based on phase drift w.r.t GPS
        tt1=tt1+5; //IK: correction based on phase drift w.r.t GPS
        NOts0=((ts0 & 0x40000000)>0); // check overflow bit
        NOts1=((ts1 & 0x40000000)>0);
        if((ts0 & 0x80000000)>0) {ts0=0x0; ts0_ref=tt0; ts0_ref_MEM[t->dstmac[5]]=tt0; 
                                  ts0_ref_AVE[t->dstmac[5]]=ts0_ref_AVE[t->dstmac[5]]+ts0_ref_MEM[t->dstmac[5]]; (ts0_ref_IND[t->dstmac[5]])++;} 
        else { ts0=tt0; ts0_ref=ts0_ref_MEM[t->dstmac[5]]; }
        if((ts1 & 0x80000000)>0) {ts1=0x0; ts1_ref=tt1; ts1_ref_MEM[t->dstmac[5]]=tt1;} else { ts1=tt1; ts1_ref=ts1_ref_MEM[t->dstmac[5]]; }

	if(t->Verbose) printf("T0=%u ns, T1=%u ns T0_ref=%u ns  T1_ref=%u ns \n",ts0,ts1,ts0_ref,ts1_ref);
//	printf(" ADC[32]:\n"); 

        for(kk=0; kk<32; kk++) if (CHAN_MASK & (1<<kk))
		{
		adc=*(UShort_t*)(&(t->gpkt).Data[jj]); jj++; jj++;  
//		printf("%04u ",adc);
	        if(t->dstmac[5] == t->macs[BoardToMon][5])
			{ 
				hst[kk]->Fill(adc);
				hcprof->Fill(kk,adc);
                                if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==6) {   
                                hevdisp->SetBinContent(kk,NEVDISP,adc);
				}
			}
                chg[kk]=adc;
		} //if(jj>=(truncat-18)) return;}
                else {chg[kk]=0; jj+=2;}

                                if(t->dstmac[5] == t->macs[BoardToMon][5]) 
 				if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==6) {   
				 for(int evi=1; evi<=NEVDISP;evi++) for(kk=0; kk<32; kk++)
				  {
                            		hevdisp->SetBinContent(kk,evi-1,hevdisp->GetBinContent(kk,evi));
				  }
				}
  
 //       printf("\n");
        mac5=t->dstmac[5];
      // printf("Filling tree with mac5=0x%02x\n",mac5);
       gts0[mac5]->SetPoint(gts0[mac5]->GetN(),gts0[mac5]->GetN(),ts0_ref-1e9);
       if(ts1!=0) gts1[mac5]->SetPoint(gts1[mac5]->GetN(),gts1[mac5]->GetN(),ts1);
        tr->Fill();
	//Adding in thermocouple fill 
	tnow = time(NULL);
	if ( (tnow - tlast) > 15 ) {
	  printf("would be logging thermocouples...\n");
	  printf("event %d\n",evs);
	  oneread = thermocouples();
	  tc->Fill();
	  tlast = tnow;
	}
if(t->dstmac[5] == t->macs[BoardToMon][5])
{
        evs++;
        evspack++; 
        evsperrequest++; 
        total_lost+=lostinfpga; 
        ts0_ref_mon=ts0_ref;  
        ts1_ref_mon=ts1_ref;
        NOts0_mon=NOts0;  
        NOts1_mon=NOts1;  
}
        }
  //        printf("Packet: events %d\n", evspack);
	  
}

void UpdateHisto()
{
    chan=fNumberEntry886->GetNumber();
    for(int y=0;y<8;y++) for(int x=0;x<4;x++) {c->cd(y*4+x+1); gPad->SetLogy(); hst[y*4+x]->Draw();}
    c->Update();
    c1->cd(); hst[chan]->Draw();
    c1->Update();
    c3->cd(1);
    gts0[t->macs[0][5]]->Draw("AL");
    gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetRangeUser(-100,100);
    gts0[t->macs[0][5]]->GetHistogram()->GetXaxis()->SetTitle("Event number");
    gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetTitle("TS0 period deviation from 1s, ns");
 for(int feb=0; feb<t->nclients; feb++)  { gts0[t->macs[feb][5]]->Draw("sameL"); }    
    c3->cd(2);
    gts1[t->macs[0][5]]->Draw("AL");
     gts1[t->macs[0][5]]->GetHistogram()->GetXaxis()->SetTitle("Event number");
    gts1[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetTitle("TS1, ns");
   //gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetRangeUser(-100,100);
 for(int feb=0; feb<t->nclients; feb++)  { gts1[t->macs[feb][5]]->Draw("sameL"); }    
    c3->Update();
    c4->cd();
    hcprof->Draw("hist");
    c4->Update();
    c5->cd();
    grevrate->Draw("AL");
    grevrate->GetHistogram()->GetXaxis()->SetTitle("Poll Nr.");
    grevrate->GetHistogram()->GetYaxis()->SetTitle("Event rate, kHz");
    c5->Update();
    c6->cd();
    hevdisp->Draw("colz");
    c6->Update();

}

void StopDAQ()
{
  t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_GEN_INIT,0,buf);  
  HVOF();
}


void StartDAQ(int nev=0)
{
t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_GEN_INIT,1,buf); //reset buffer
  t->SendCMD(t->dstmac,FEB_GEN_INIT,2,buf); //set DAQ_Enable flag on FEB
  HVON();
  DAQ(nev);
  RunOn=0;
  StopDAQ();
  HVOF();
}


void DAQ(int nev=0)
{
  char str1[32];
  char str2[32];
  evs=0;
  int nevv=nev;
  double avts0;
  int deltaVCXO;
 // evs_notfirst=0;
  int notok=0;
  int ok=0;
//  bool first_request=true;
  total_lost=0;
  RunOn=1;
  float PollPeriod;
  float rate;
//  UpdateConfig();
 // t->SendCMD(t->dstmac,FEB_GEN_HVON,0,buf);
 // printf("nevv=%d, evs=%d\n",nevv,evs);
  tm0=time(NULL);
  tm1=time(NULL);
  while(RunOn==1 && (nevv==0 || evs<nevv) && (fNumberEntryTME->GetNumber()==0 || tm1-tm0 < fNumberEntryTME->GetNumber() ))
  {
   
 // printf("nevv=%d, evs=%d\n",nevv,evs);
   BenchMark->Show("Poll");
   PollPeriod=BenchMark->GetRealTime("Poll");
   BenchMark->Reset();
   BenchMark->Start("Poll");
//Perform VCXO correction
for(int feb=0; feb<t->nclients; feb++)
{ 
  if(ts0_ref_IND[t->macs[feb][5]]>=20) 
  { //correct one FEB VCXO
    avts0=ts0_ref_AVE[t->macs[feb][5]] / ts0_ref_IND[t->macs[feb][5]];
    deltaVCXO=-(avts0-1e9)/5.2; //derive correction increment, approx 5.2 ns per DAC LSB
 //   printf("Average period %f\n",avts0);
    VCXO_Values[feb]=VCXO_Values[feb]+deltaVCXO;
    ts0_ref_AVE[t->macs[feb][5]]=0;ts0_ref_IND[t->macs[feb][5]]=0; 
    if(fUpdateVCXO->IsOn()) {
    printf("------------------ For board %d : Average period %f, Set VCXO  (0x%02x) to %d (+%d)\n",feb,avts0, t->macs[feb][5], VCXO_Values[feb],deltaVCXO);
    t->VCXO=VCXO_Values[feb];
    t->SendCMD(t->macs[feb],FEB_SET_RECV,VCXO_Values[feb],t->srcmac);
    }
  }
  
}
    chan=fNumberEntry886->GetNumber();
    rate=GetTriggerRate()/1e3;
    sprintf(str1,"Trigger %2.3f kHz",rate);
              grevrate->SetPoint(grevrate->GetN(),grevrate->GetN(),rate); 

          if(rate<3.6) fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xbbffbb);
          else if (rate<5.0) fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xffbbbb);
          else if (rate<7.0) fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xff9999);
          else if (rate<10.0) fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xff7777);
          else fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xff3333);
    
//    fLabel->SetText(str1);
    fStatusBar739->SetText(str1,0);
for(int feb=0; feb<t->nclients; feb++)
{
SetDstMacByIndex(feb);
    mac5=t->dstmac[5];
    ok=t->SendCMD(t->dstmac,FEB_RD_CDR,0,buf);
}

    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==1) {   
     for(int y=0;y<8;y++) for(int x=0;x<4;x++) {c->cd(y*4+x+1); gPad->SetLogy(); hst[y*4+x]->Draw();}
     c->Update();
    }
    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==2) {   
     c1->cd(); hst[chan]->Draw();
     c1->Update();
    }
    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==3) {   
     c3->cd(1);
     gts0[t->macs[0][5]]->Draw("AL");
     gts0[t->macs[0][5]]->GetHistogram()->GetXaxis()->SetTitle("Event number");
     gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetTitle("TS0 period deviation from 1s, ns");

     gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetRangeUser(-100,100);
     for(int feb=0; feb<t->nclients; feb++)  { gts0[t->macs[feb][5]]->Draw("sameL"); }    
     c3->cd(2);
     gts1[t->macs[0][5]]->Draw("AL");
     gts1[t->macs[0][5]]->GetHistogram()->GetXaxis()->SetTitle("Event number");
     gts1[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetTitle("TS1, ns");

     //gts0[t->macs[0][5]]->GetHistogram()->GetYaxis()->SetRangeUser(-100,100);
     for(int feb=0; feb<t->nclients; feb++)  { gts1[t->macs[feb][5]]->Draw("sameL"); }    
     c3->Update();
    }
    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==4) {   
     c4->cd();
     hcprof->Draw("hist");
     c4->Update();
    }
    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==5) {   
     c5->cd();
     grevrate->Draw("AL");
    grevrate->GetHistogram()->GetXaxis()->SetTitle("Poll Nr.");
    grevrate->GetHistogram()->GetYaxis()->SetTitle("Event rate, kHz");
    c5->Update();
    }
    if(fUpdateHisto->IsOn() && fTab683->GetCurrent()==6) {   
     c6->cd();
     hevdisp->Draw("colz");
     c6->Update();
    }
    


    gSystem->ProcessEvents();
          printf("Per request: %d events acquired, overwritten (flags field of last event) %d\n",evsperrequest,overwritten);
          if(nevv>0) printf("%d events to go...\n",nevv-evs);
          sprintf(str1,"Poll: %d events acquired in %2.2f sec.",evsperrequest,PollPeriod);
         if(evsperrequest>0) fStatusBar739->GetBarPart(1)->SetBackgroundColor(0xbbffbb);
         else fStatusBar739->GetBarPart(1)->SetBackgroundColor(0xffaaaa);

          fStatusBar739->SetText(str1,1);
          sprintf(str1,"Evs lost FPGA:%d CPU:%d ",lostinfpga,overwritten);
          if(overwritten==0) fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xbbffbb);
          else if (overwritten<10) fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xffbbbb);
          else if (overwritten<100) fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xff9999);
          else if (overwritten<1000) fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xff7777);
          else fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xff3333);
          fStatusBar739->SetText(str1,2);
         if(NOts0_mon) { sprintf(str1,"PPS missing!"); fStatusBar739->GetBarPart(3)->SetBackgroundColor(0xffaaaa);} 
         else { sprintf(str1,"PPS period %d ns",ts0_ref_mon); fStatusBar739->GetBarPart(3)->SetBackgroundColor(0xbbffbb);} 
          fStatusBar739->SetText(str1,3);
         if(NOts1_mon) { sprintf(str1,"SPILL trig missing!"); fStatusBar739->GetBarPart(4)->SetBackgroundColor(0xffaaaa);} 
         else { sprintf(str1,"SPILL trig period %d ns",ts1_ref_mon);fStatusBar739->GetBarPart(4)->SetBackgroundColor(0xbbffbb);} 
          fStatusBar739->SetText(str1,4);
            sprintf(str1, "Overal: %d acquired, %d (%2.1f\%%) lost",evs,total_lost, (100.*total_lost/(evs+total_lost)));
          fStatusBar739->SetText(str1,5);

  //         if(first_request ) {first_request=false; total_lost=0;}
          total_lost+=overwritten; //evs_notfirst+=evsperrequest; }//skip lost events from the very first request
     //     if(evsperrequest==0) first_request== true;
 //      printf("Debug: total_lost=%d \n",total_lost);
          overwritten=0;
          evsperrequest=0;
     tm1=time(NULL);

  }
          fStatusBar739->GetBarPart(0)->SetBackgroundColor(0xffffff); 
          fStatusBar739->GetBarPart(1)->SetBackgroundColor(0xffffff); 
          fStatusBar739->GetBarPart(2)->SetBackgroundColor(0xffffff); 
          fStatusBar739->GetBarPart(3)->SetBackgroundColor(0xffffff); 
          fStatusBar739->GetBarPart(4)->SetBackgroundColor(0xffffff); 
          fStatusBar739->GetBarPart(5)->SetBackgroundColor(0xffffff); 
   
 // t->SendCMD(t->dstmac,FEB_GEN_HVOF,0,buf);
  printf("Overal per DAQ call: %d events acquired, %d (%2.1f\%%) lost (skipping first request).\n",evs,total_lost, (100.*total_lost/(evs+total_lost)));
           sprintf(str1, "Overal: %d acquired, %d (%2.1f\%%) lost",evs,total_lost, (100.*total_lost/(evs+total_lost)));
          fStatusBar739->SetText(str1,5);
    for(int y=0;y<8;y++) for(int x=0;x<4;x++) {c->cd(y*4+x+1); gPad->SetLogy(); hst[y*4+x]->Draw();}
    c->Update();


}

void Reset()
{
     for(int y=0;y<8;y++) for(int x=0;x<4;x++) { hst[y*4+x]->Reset();}
     c1->cd(); hst[chan]->Draw();
    c1->Update();
    for(int y=0;y<8;y++) for(int x=0;x<4;x++) {c->cd(y*4+x+1); gPad->SetLogy(); hst[y*4+x]->Draw();}
    c->Update();
    tr->Reset();
    tc->Reset();
    evs=0;
    total_lost=0;
 for(int feb=0; feb<t->nclients; feb++)  { gts0[t->macs[feb][5]]->Set(0); }
 for(int feb=0; feb<t->nclients; feb++)  { gts1[t->macs[feb][5]]->Set(0); }
      c3->cd(1);
//    hcprof->Draw("hist");
    c3->Update();
 
    hcprof->Reset(); 
      c4->cd();
    hcprof->Draw("hist");
    c4->Update();
    grevrate->Set(0); 
      c5->cd();
    grevrate->Draw("AL");
    grevrate->GetHistogram()->GetXaxis()->SetTitle("Poll Nr.");
    grevrate->GetHistogram()->GetYaxis()->SetTitle("Event rate, kHz");
    c5->Update();
    hevdisp->Reset(); 
      c6->cd();
    hevdisp->Draw("colz");
    c6->Update();

  //  evs_notfirst=0;
}

void All()
{
       for(int y=0;y<8;y++) for(int x=0;x<4;x++) { hst[y*4+x]->Draw("same");}
}

void HVON()
{
  //for(int feb=0; feb<t->nclients; feb++)
  //{
  //SetDstMacByIndex(feb);
  t->dstmac[5]=0xff; //Broadcast
  t->SendCMD(t->dstmac,FEB_GEN_HVON,0x0000,buf);
  cout << "HIGH VOLTAGE IS ON" << endl;
  Pixel_t red;
  gClient->GetColorByName("red", red);
  bHVStatus->ChangeBackground(red);
  bHVStatus->SetText("HV ON");
  bHVStatus->SetState(kButtonDown);
  //}
}

void HVOF()
{
//for(int feb=0; feb<t->nclients; feb++)
//{
//SetDstMacByIndex(feb);
t->dstmac[5]=0xff; //Broadcast
 t->SendCMD(t->dstmac,FEB_GEN_HVOF,0x0000,buf);
 cout << "HIGH VOLTAGE IS OFF" << endl;
 Pixel_t green;
 gClient->GetColorByName("green", green);
 bHVStatus->ChangeBackground(green);
 bHVStatus->SetText("HV OFF");
 bHVStatus->SetState(kButtonUp);
//}
}

float GetTriggerRate()
{
 float retval;
 t->SendCMD(t->macs[BoardToMon],FEB_GET_RATE,0,buf);
 retval=*((float*)(t->gpkt.Data));
 return retval;
}
void ThresholdScan(UShort_t start, UShort_t stop)
{
UShort_t thr;
Int_t i=0; 
if(gr==0) gr=new TGraph();
  SetDstMacByIndex(BoardToMon);
for( thr=start; thr<=stop; thr++)
 {
  SetThresholdDAC1(thr);
  SetThresholdDAC2(thr);
  gSystem->Sleep(2000); //to let FEB update rate
  gr->SetPoint(i,thr,GetTriggerRate());
  i++;
  c1->cd(); gr->Draw("AL");
  c1->Update(); 
 }
}

void SetThresholdDAC1(UShort_t dac1)
{
int offset=1107;
for(int i=0; i<10;i++)
{ 
  if( (dac1 & 1)>0) ConfigSetBit(bufSCR,1144,offset+9-i,kTRUE);
  else ConfigSetBit(bufSCR,1144,offset+9-i,kFALSE);
  dac1= dac1 >> 1;
}
//UShort_t* pdac=(UShort_t*)(&bufSCR[3]);
// printf("%4x",((*pdac)>>3)&0x3ff)
//*pdac=*pdac & 0xE007; //clean bits of ADC1
//*pdac=*pdac | ((dac1 << 3) & 0x1FF8);
t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
}

UShort_t GetThresholdDAC1()
{
int offset=1107;
UShort_t dac1=0;
for(int i=0; i<10;i++)
{ 
  dac1= dac1 >> 1;
  if(ConfigGetBit(bufSCR,1144,offset+9-i)) dac1=dac1 | 0x0200;
}
return dac1;
}

void SetThresholdDAC2(UShort_t dac1)
{
int offset=1117;
for(int i=0; i<10;i++)
{ 
  if( (dac1 & 1)>0) ConfigSetBit(bufSCR,1144,offset+9-i,kTRUE);
  else ConfigSetBit(bufSCR,1144,offset+9-i,kFALSE);
  dac1= dac1 >> 1;
}
//UShort_t* pdac=(UShort_t*)(&bufSCR[3]);
// printf("%4x",((*pdac)>>3)&0x3ff)
//*pdac=*pdac & 0xE007; //clean bits of ADC1
//*pdac=*pdac | ((dac1 << 3) & 0x1FF8);
t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
}



   void GUI_UpdateThreshold()
    {
     UShort_t dac1;
     dac1=fNumberEntry755->GetNumber();
//	for(int feb=0; feb<t->nclients; feb++)
//	{
//	SetDstMacByIndex(feb);
t->dstmac[5]=0xff; //Broadcast
      	SetThresholdDAC1(dac1);
      	SetThresholdDAC2(dac1);
//        }
    }
   void GUI_UpdateVCXO()
    {
     t->VCXO=fNumberEntry75->GetNumber();
     t->SendCMD(t->macs[BoardToMon],FEB_SET_RECV,fNumberEntry75->GetNumber(),t->srcmac);    
    }

  void UpdateVCXOAllFEBs()
    {
     for(int feb=0; feb<t->nclients; feb++)   t->SendCMD(t->macs[feb],FEB_SET_RECV,VCXO_Values[feb],t->srcmac);    
    }
  
void UpdateBoardMonitor()
{
  BoardToMon=fNumberEntry8869->GetNumber();  
  char sttr[32];
  sprintf(sttr,"Mon FEB 0x%2x %d",t->macs[BoardToMon][5],t->macs[BoardToMon][5]);
  printf("Monitoring FEB mac5 0x%2x %d\n",t->macs[BoardToMon][5],t->macs[BoardToMon][5]);
  fLabel7->SetText(sttr); 
  //ResetHistos();
       for(int y=0;y<8;y++) for(int x=0;x<4;x++) { hst[y*4+x]->Reset();}
 
}

void FEBGUI()
{

   // main frame
   TGMainFrame *fMainFrame1314 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame1314->SetName("fMainFrame1314");
   fMainFrame1314->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1560 = new TGCompositeFrame(fMainFrame1314,1329,789+100,kVerticalFrame);
   fMainFrame1560->SetName("fMainFrame1560");
   fMainFrame1560->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1241 = new TGCompositeFrame(fMainFrame1560,1329,789+100,kVerticalFrame);
   fMainFrame1241->SetName("fMainFrame1241");

   // vertical frame
   TGVerticalFrame *fVerticalFrame734 = new TGVerticalFrame(fMainFrame1241,1327,787+100,kVerticalFrame);
   fVerticalFrame734->SetName("fVerticalFrame734");

   // status bar
   Int_t parts[] = {15, 15, 15, 15, 15, 25};
   fStatusBar739 = new TGStatusBar(fVerticalFrame734,1327,18);
   fStatusBar739->SetName("fStatusBar739");
   fStatusBar739->SetParts(parts, 6);
   fVerticalFrame734->AddFrame(fStatusBar739, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));



   // horizontal frame
   TGHorizontalFrame *fHorizontalFrame768 = new TGHorizontalFrame(fVerticalFrame734,1350,765+100,kHorizontalFrame);
   fHorizontalFrame768->SetName("fHorizontalFrame768");

   // "DAQ FEB controls" group frame
   TGGroupFrame *fGroupFrame679 = new TGGroupFrame(fHorizontalFrame768,"DAQ FEB controls");
   TGTextButton *fTextButton680 = new TGTextButton(fGroupFrame679,"Update Config");
   fTextButton680->SetTextJustify(36);
   fTextButton680->SetMargins(0,0,0,0);
   fTextButton680->SetWrapLength(-1);
   fTextButton680->Resize(123,22);
   fTextButton680->SetCommand("UpdateConfig()");
   fGroupFrame679->AddFrame(fTextButton680, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,37,0));
   TGTextButton *fTextButton681 = new TGTextButton(fGroupFrame679,"Start DAQ");
   fTextButton681->SetTextJustify(36);
   fTextButton681->SetMargins(0,0,0,0);
   fTextButton681->SetWrapLength(-1);
   fTextButton681->Resize(123,22);
   fTextButton681->SetCommand("if(RunOn==0) StartDAQ();");
   fGroupFrame679->AddFrame(fTextButton681, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fNumberEntryTME= new TGNumberEntry(fGroupFrame679, (Double_t) 1200,6,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1e9);
   fGroupFrame679->AddFrame(fNumberEntryTME, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   //HV status button. 
   bHVStatus = new TGTextButton(fGroupFrame679,"High Voltage",418);
   bHVStatus->SetTextJustify(36);
   bHVStatus->SetMargins(0,0,0,0);
   bHVStatus->SetWrapLength(-1);
   bHVStatus->Resize(123,22);
   bHVStatus->Connect("Clicked()",0,0,"FEBGUI()");
   fGroupFrame679->AddFrame(bHVStatus, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));

   TGTextButton *fTextButton682 = new TGTextButton(fGroupFrame679,"Stop DAQ");
   fTextButton682->SetTextJustify(36);
   fTextButton682->SetMargins(0,0,0,0);
   fTextButton682->SetWrapLength(-1);
   fTextButton682->Resize(123,22);
   fTextButton682->SetCommand("RunOn=0; StopDAQ();");
   fGroupFrame679->AddFrame(fTextButton682, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,2,2));

   TGTextButton *fTextButton788 = new TGTextButton(fGroupFrame679,"Reset Histos");
   fTextButton788->SetTextJustify(36);
   fTextButton788->SetMargins(0,0,0,0);
   fTextButton788->SetWrapLength(-1);
   fTextButton788->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton788, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fTextButton788->SetCommand("Reset()");

   TGTextButton *fTextButton78 = new TGTextButton(fGroupFrame679,"SiPM HV ON");
   fTextButton78->SetTextJustify(36);
   fTextButton78->SetMargins(0,0,0,0);
   fTextButton78->SetWrapLength(-1);
   fTextButton78->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton78, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fTextButton78->SetCommand("HVON()");

   TGTextButton *fTextButton88 = new TGTextButton(fGroupFrame679,"SiPM HV OFF");
   fTextButton88->SetTextJustify(36);
   fTextButton88->SetMargins(0,0,0,0);
   fTextButton88->SetWrapLength(-1);
   fTextButton88->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton88, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fTextButton88->SetCommand("HVOF()");

   TGTextButton *fTextButton881 = new TGTextButton(fGroupFrame679,"Profile Test");
   fTextButton881->SetTextJustify(36);
   fTextButton881->SetMargins(0,0,0,0);
   fTextButton881->SetWrapLength(-1);
   fTextButton881->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton881, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fTextButton881->SetCommand("MakeProfileTest()");


   fNumberEntry755 = new TGNumberEntry(fGroupFrame679, (Double_t) 250,6,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1023);
   fGroupFrame679->AddFrame(fNumberEntry755, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,62,2));
   TGTextButton *fTextButton931 = new TGTextButton(fGroupFrame679,"Set Threshold DAC1");
   fTextButton931->SetTextJustify(36);
   fTextButton931->SetMargins(0,0,0,0);
   fTextButton931->SetWrapLength(-1);
   fTextButton931->Resize(123,22);
   fTextButton931->SetCommand("GUI_UpdateThreshold()");
   fGroupFrame679->AddFrame(fTextButton931, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,2,2));


   fGroupFrame679->SetLayoutManager(new TGVerticalLayout(fGroupFrame679));
   fGroupFrame679->Resize(155,761);
   fHorizontalFrame768->AddFrame(fGroupFrame679, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));

   TGLabel *fLabel764 = new TGLabel(fGroupFrame679,"Select channel");
   fLabel764->SetTextJustify(36);
   fLabel764->SetMargins(0,0,0,0);
   fLabel764->SetWrapLength(-1);
   fGroupFrame679->AddFrame(fLabel764, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,30,2));
   fNumberEntry886 = new TGNumberEntry(fGroupFrame679, (Double_t) 4,3,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,31);
   fGroupFrame679->AddFrame(fNumberEntry886, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX ));
   fNumberEntry886->SetCommand("UpdateHisto()");



   fUpdateHisto = new TGCheckButton(fGroupFrame679,"Auto update histograms");
   fUpdateHisto->SetTextJustify(36);
   fUpdateHisto->SetMargins(0,0,0,0);
   fUpdateHisto->SetWrapLength(-1);
   fUpdateHisto->SetCommand("UpdateHisto()");
   fGroupFrame679->AddFrame(fUpdateHisto, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUpdateHisto->SetOn();

   fUpdateVCXO = new TGCheckButton(fGroupFrame679,"Auto correct VCXOs");
   fUpdateVCXO->SetTextJustify(36);
   fUpdateVCXO->SetMargins(0,0,0,0);
   fUpdateVCXO->SetWrapLength(-1);
//   fUpdateVCXO->SetCommand("UpdateHisto()");
   fGroupFrame679->AddFrame(fUpdateVCXO, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUpdateVCXO->SetOn(0);
   fUpdateVCXO->SetEnabled(0);    

   TGCheckButton* fVerbose = new TGCheckButton(fGroupFrame679,"Verbose console output");
   fVerbose->SetTextJustify(36);
   fVerbose->SetMargins(0,0,0,0);
   fVerbose->SetWrapLength(-1);
   fVerbose->SetCommand("if(t->Verbose==1) t->Verbose=0; else t->Verbose=1;");
   fGroupFrame679->AddFrame(fVerbose, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   //fVerbose->SetOff();


 /*  fLabel = new TGLabel(fGroupFrame679,"Trigger rate: 0 Hz");
   fLabel->SetTextJustify(36);
   fLabel->SetMargins(0,0,0,0);
   fLabel->SetWrapLength(-1);
   fGroupFrame679->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft  | kLHintsExpandX,0,0,39,0));
*/

   fNumberEntry75 = new TGNumberEntry(fGroupFrame679, (Double_t) 500,6,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1023);
   fGroupFrame679->AddFrame(fNumberEntry75, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,42,2));
   TGTextButton *fTextButton93 = new TGTextButton(fGroupFrame679,"Set VCXO correction");
   fTextButton93->SetTextJustify(36);
   fTextButton93->SetMargins(0,0,0,0);
   fTextButton93->SetWrapLength(-1);
   fTextButton93->Resize(123,22);
   fTextButton93->SetCommand("GUI_UpdateVCXO()");
   fGroupFrame679->AddFrame(fTextButton93, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,2,2));




   TGTextButton *fTextButton111 = new TGTextButton(fGroupFrame679,"Save data tree");
   fTextButton111->SetTextJustify(36);
   fTextButton111->SetMargins(0,0,0,0);
   fTextButton111->SetWrapLength(-1);
   fTextButton111->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton111, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));


   TString rootfilename = "mppc.root";
   
   //fTextButton111->SetCommand("tr->SaveAs(\"mppc.root\");tc->Write();mppc.root->Close()");
   fTextButton111->SetCommand("tr->SaveAs(\""+rootfilename+"\");TFile *fff = new TFile(\""+rootfilename+"\",\"update\");tc->Write(); fff->Close()");

   fLabel7 = new TGLabel(fGroupFrame679,"0xHH");
   fLabel7->SetTextJustify(36);
   fLabel7->SetMargins(0,0,0,0);
   fLabel7->SetWrapLength(-1);
   fGroupFrame679->AddFrame(fLabel7, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,39,2));

   TGLabel *fLabel769 = new TGLabel(fGroupFrame679,"Select FEB");
   fLabel769->SetTextJustify(36);
   fLabel769->SetMargins(0,0,0,0);
   fLabel769->SetWrapLength(-1);
   fGroupFrame679->AddFrame(fLabel769, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,30,2));
   fNumberEntry8869 = new TGNumberEntry(fGroupFrame679, (Double_t) 0,3,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 3,0,255);
   fGroupFrame679->AddFrame(fNumberEntry8869, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX ));
  // fNumberEntry8869->SetCommand("UpdateBoardMonitor()");
   fNumberEntry8869->Connect("ValueSet(Long_t)", 0, 0,  "UpdateBoardMonitor()");

   TGTextButton *fTextButton1188 = new TGTextButton(fGroupFrame679,"Rescan network");
   fTextButton1188->SetTextJustify(36);
   fTextButton1188->SetMargins(0,0,0,0);
   fTextButton1188->SetWrapLength(-1);
   fTextButton1188->Resize(123,22);
   fGroupFrame679->AddFrame(fTextButton1188, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
   fTextButton1188->SetCommand("RescanNet()");



   // tab widget
   fTab683 = new TGTab(fHorizontalFrame768,1187,761+100);

   // container of "Configuration"
   TGCompositeFrame *fCompositeFrame686;
   fCompositeFrame686 = fTab683->AddTab("Configuration");
   fCompositeFrame686->SetLayoutManager(new TGHorizontalLayout(fCompositeFrame686));
   char str[32];

   TGVButtonGroup* fButtonGroup2=new TGVButtonGroup(fCompositeFrame686,"Enable Amplifier");
   for(int i=0;i<32;i++)
   {
   sprintf(str,"ch%d",i);
   fChanEnaAmp[i] = new TGCheckButton(fButtonGroup2,str);
   fChanEnaAmp[i]->SetTextJustify(36);
   fChanEnaAmp[i]->SetMargins(0,0,0,0);
   fChanEnaAmp[i]->SetWrapLength(-1);
    fChanEnaAmp[i]->SetCommand("SendConfig()");
   }
   fChanEnaAmp[32] = new TGCheckButton(fButtonGroup2,"All");
   fChanEnaAmp[32]->SetTextJustify(36);
   fChanEnaAmp[32]->SetMargins(0,0,0,0);
   fChanEnaAmp[32]->SetWrapLength(-1);
    fChanEnaAmp[32]->SetCommand("SendAllChecked()");
  fChanEnaAmp[33] = new TGCheckButton(fButtonGroup2,"None");
   fChanEnaAmp[33]->SetTextJustify(36);
   fChanEnaAmp[33]->SetMargins(0,0,0,0);
   fChanEnaAmp[33]->SetWrapLength(-1);
    fChanEnaAmp[33]->SetCommand("SendAllUnChecked()");
   
   fCompositeFrame686->AddFrame(fButtonGroup2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fButtonGroup2->Show();

   TGVButtonGroup* fButtonGroup3=new TGVButtonGroup(fCompositeFrame686,"Enable Trigger");
   for(int i=0;i<32;i++)
   {
   sprintf(str,"ch%d",i);
   fChanEnaTrig[i] = new TGCheckButton(fButtonGroup3,str);
   fChanEnaTrig[i]->SetTextJustify(36);
   fChanEnaTrig[i]->SetMargins(0,0,0,0);
   fChanEnaTrig[i]->SetWrapLength(-1);
    fChanEnaTrig[i]->SetCommand("SendConfig()");
   }
    fChanEnaTrig[32] = new TGCheckButton(fButtonGroup3,"OR32");
   fChanEnaTrig[32]->SetTextJustify(36);
   fChanEnaTrig[32]->SetMargins(0,0,0,0);
   fChanEnaTrig[32]->SetWrapLength(-1);
    fChanEnaTrig[32]->SetCommand("SendConfig()");
  
   fCompositeFrame686->AddFrame(fButtonGroup3, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fButtonGroup3->Show();


   TGVButtonGroup* fButtonGroup1=new TGVButtonGroup(fCompositeFrame686,"Probe register");
   for(int i=0;i<32;i++)
   {
   sprintf(str,"ch%d",i);
   fChanProbe[i] = new TGRadioButton(fButtonGroup1,str);
   fChanProbe[i]->SetTextJustify(36);
   fChanProbe[i]->SetMargins(0,0,0,0);
   fChanProbe[i]->SetWrapLength(-1);
    fChanProbe[i]->SetCommand("SendConfig()");
   }
   fChanProbe[32] = new TGRadioButton(fButtonGroup1,"None");
   fChanProbe[32]->SetTextJustify(36);
   fChanProbe[32]->SetMargins(0,0,0,0);
   fChanProbe[32]->SetWrapLength(-1);
    fChanProbe[32]->SetCommand("SendConfig()");
   fCompositeFrame686->AddFrame(fButtonGroup1, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fButtonGroup1->SetRadioButtonExclusive(kTRUE);
    fButtonGroup1->Show();
    fChanProbe[4]->SetOn();

  // TGGroupFrame* fGainsGroup = new TGGroupFrame(
    TGGroupFrame* fGains=new TGGroupFrame(fCompositeFrame686,"HG preamp gain/bias");
    //fGains->SetLayoutManager(new TGVerticalLayout(fGains));
   fGains->SetLayoutManager(new TGMatrixLayout(fGains, 32, 3, 0, kLHintsLeft | kLHintsTop));
   for(int i=0;i<32;i++)
   {
   sprintf(str," CH %d",i);
   fChanGain[i] = new TGNumberEntry(fGains, (Double_t) i,2,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,64);
    fChanGain[i]->Connect("ValueSet(Long_t)", 0, 0,  "SendConfig()");
    fChanGain[i]->SetHeight(20);
   fChanBias[i] = new TGNumberEntry(fGains, (Double_t) i,3,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,256);
    fChanBias[i]->Connect("ValueSet(Long_t)", 0, 0,  "SendConfig()");
    fChanBias[i]->SetHeight(20);
    fGains->AddFrame(new TGLabel(fGains,str));
    fGains->AddFrame(fChanGain[i]);
    fGains->AddFrame(fChanBias[i]);

   }
   fCompositeFrame686->AddFrame(fGains, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


   // container of "All histos"
   TGCompositeFrame *fCompositeFrame720;
   fCompositeFrame720 = fTab683->AddTab("All histos");
   fCompositeFrame720->SetLayoutManager(new TGVerticalLayout(fCompositeFrame720));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas721 = new TRootEmbeddedCanvas(0,fCompositeFrame720,1179,732+100);
   Int_t wfRootEmbeddedCanvas721 = fRootEmbeddedCanvas721->GetCanvasWindowId();
   c = new TCanvas("c", 10, 10, wfRootEmbeddedCanvas721);    c->Divide(4,8);

   fRootEmbeddedCanvas721->AdoptCanvas(c);
   fCompositeFrame720->AddFrame(fRootEmbeddedCanvas721, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


   // container of "One channel"
   TGCompositeFrame *fCompositeFrame735;
   fCompositeFrame735 = fTab683->AddTab("One channel");
   fCompositeFrame735->SetLayoutManager(new TGVerticalLayout(fCompositeFrame735));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas736 = new TRootEmbeddedCanvas(0,fCompositeFrame735,1179,732+100);
   Int_t wfRootEmbeddedCanvas736 = fRootEmbeddedCanvas736->GetCanvasWindowId();
   c1 = new TCanvas("c1", 10, 10, wfRootEmbeddedCanvas736);
   fRootEmbeddedCanvas736->AdoptCanvas(c1);
   fCompositeFrame735->AddFrame(fRootEmbeddedCanvas736, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

   // container of "Timing data"
   TGCompositeFrame *fCompositeFrame7351;
   fCompositeFrame7351 = fTab683->AddTab("Timing data");
   fCompositeFrame7351->SetLayoutManager(new TGVerticalLayout(fCompositeFrame7351));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas7361 = new TRootEmbeddedCanvas(0,fCompositeFrame7351,1179,732+100);
   Int_t wfRootEmbeddedCanvas7361 = fRootEmbeddedCanvas7361->GetCanvasWindowId();
   c3 = new TCanvas("c3", 10, 10, wfRootEmbeddedCanvas7361);
   fRootEmbeddedCanvas7361->AdoptCanvas(c3);
   fCompositeFrame7351->AddFrame(fRootEmbeddedCanvas7361, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));
   c3->Divide(1,2);
   c3->cd(1)->SetGridx(1);
   c3->cd(1)->SetGridy(1);
  c3->cd(2)->SetGridx(1);
   c3->cd(2)->SetGridy(1);

   // container of "TChannel profile"
   TGCompositeFrame *fCompositeFrame7352;
   fCompositeFrame7352 = fTab683->AddTab("Channel profile");
   fCompositeFrame7352->SetLayoutManager(new TGVerticalLayout(fCompositeFrame7352));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas7362 = new TRootEmbeddedCanvas(0,fCompositeFrame7352,1179,732+100);
   Int_t wfRootEmbeddedCanvas7362 = fRootEmbeddedCanvas7362->GetCanvasWindowId();
   c4 = new TCanvas("c4", 10, 10, wfRootEmbeddedCanvas7362);
   fRootEmbeddedCanvas7362->AdoptCanvas(c4);
   fCompositeFrame7352->AddFrame(fRootEmbeddedCanvas7362, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));
   c4->SetGridx(1);
   c4->SetGridy(1);

   // container of "Rate history"
   TGCompositeFrame *fCompositeFrame73521;
   fCompositeFrame73521 = fTab683->AddTab("Event rate");
   fCompositeFrame73521->SetLayoutManager(new TGVerticalLayout(fCompositeFrame73521));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas73621 = new TRootEmbeddedCanvas(0,fCompositeFrame73521,1179,732+100);
   Int_t wfRootEmbeddedCanvas73621 = fRootEmbeddedCanvas73621->GetCanvasWindowId();
   c5 = new TCanvas("c5", 10, 10, wfRootEmbeddedCanvas73621);
   fRootEmbeddedCanvas73621->AdoptCanvas(c5);
   fCompositeFrame73521->AddFrame(fRootEmbeddedCanvas73621, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));
   c5->SetGridx(1);
   c5->SetGridy(1);

   // container of "Event display"
   TGCompositeFrame *fCompositeFrame73522;
   fCompositeFrame73522 = fTab683->AddTab("Event display");
   fCompositeFrame73522->SetLayoutManager(new TGVerticalLayout(fCompositeFrame73522));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas73622 = new TRootEmbeddedCanvas(0,fCompositeFrame73522,1179,732+100);
   Int_t wfRootEmbeddedCanvas73622 = fRootEmbeddedCanvas73622->GetCanvasWindowId();
   c6 = new TCanvas("c6", 10, 10, wfRootEmbeddedCanvas73622);
   fRootEmbeddedCanvas73622->AdoptCanvas(c6);
   fCompositeFrame73522->AddFrame(fRootEmbeddedCanvas73622, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));


   fTab683->SetTab(2);

   fTab683->Resize(fTab683->GetDefaultSize());
   fHorizontalFrame768->AddFrame(fTab683, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

   fVerticalFrame734->AddFrame(fHorizontalFrame768, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

   fMainFrame1241->AddFrame(fVerticalFrame734, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));

   fMainFrame1560->AddFrame(fMainFrame1241, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1241->MoveResize(0,0,1329,789+100);

   fMainFrame1314->AddFrame(fMainFrame1560, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1560->MoveResize(0,0,1329,789+100);

   fMainFrame1314->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame1314->MapSubwindows();

   fMainFrame1314->Resize(fMainFrame1314->GetDefaultSize());
   fMainFrame1314->MapWindow();
   fMainFrame1314->Resize(1329,789+100);
}  

uint8_t getCRC(uint8_t message[], uint32_t length)
{
	uint32_t i, j;
	uint8_t crc = 0;

  for (i = 0; i < length; i++)
  {
    crc ^= message[i];
    for (j = 0; j < 8; j++)
    {
      if (crc & 1)
        crc ^=  0x91; //CRC7_POLY
      crc >>= 1;
    }
  }
  return crc;
}

int FW_ptr=0;
uint8_t fwbuf[1024*1024]; //Firmware buffer
uint8_t crc7;

void ReceiveFW(int truncat)  // hook called by libFEBDTP when event is received
{
   for(int j=0;j<1024; j++) {
    if(t->Verbose) { printf("%02x ",t->gpkt.Data[j]);     if(j%16==15) printf("\n"); }
    fwbuf[FW_ptr+j]=t->gpkt.Data[j];
   }
   FW_ptr+=1024;
}

bool DumpFW(uint32_t startaddr=0, uint16_t blocks=1)
{
  bool retval=1;
 // memset(fwbuf,1024*1024,00);
  buf[0]=startaddr & 0x000000ff; 
  buf[1]=(startaddr & 0x0000ff00)>>8; 
  buf[2]=(startaddr & 0x00ff0000)>>16; 
  buf[3]=(blocks & 0x00FF);
  buf[4]=(blocks & 0xFF00) >>8;
  t->setPacketHandler(&ReceiveFW);
  FW_ptr=startaddr;
  t->SendCMD(t->dstmac,FEB_RD_FW,00,buf);//==0) //retval=0;
  t->setPacketHandler(&FillHistos);
  crc7=getCRC(&(fwbuf[startaddr]),blocks*1024);
  printf("DumpFW: Received %d bytes at 0x%08x in Flash ( 0x%08x in CPU address space)",blocks*1024,startaddr,startaddr+0x14000000);
  if(crc7==t->gpkt.REG)  printf("CRC check OK (%02x).\n",crc7);
  else  { printf("CRC check NOT OK!! received CRC=%02x, locally calculated on Host=%02x!\n",crc7,t->gpkt.REG); retval=0;}
   if(retval==0) printf("Error in DumpFW!!\n");
 return retval;
}

bool ProgramFW(uint32_t startaddr=0, uint16_t blocks=1) 
{
  bool retval=1;
  buf[0]=startaddr & 0x000000ff; 
  buf[1]=(startaddr & 0x0000ff00)>>8; 
  buf[2]=(startaddr & 0x00ff0000)>>16; 
  buf[3]=(blocks & 0x00FF);
  buf[4]=00; //(blocks & 0xFF00) >>8;
  t->dstmac[5] = t->macs[BoardToMon][5];
  if( t->SendCMD(t->dstmac,FEB_WR_FW,00,buf)==0) retval=0; //initiate flash programming
  for( int i=0;i<blocks;i++) {
    if(t->SendCMD(t->dstmac,FEB_DATA_FW,getCRC(fwbuf+i*1024,1024),fwbuf+i*1024)==0) retval=0;
  }
  if(retval==0) printf("Error in ProgramFW!!\n");
  return retval;

}

bool GetAndSaveFW(char *fname=NULL)
{
   
  bool retval=1;
   memset(fwbuf,1024*1024,00);
  for(int i=0;i<16;i++) DumpFW(i*64*1024,64);
  if(fname==NULL) return 0;
  FILE *fp=fopen(fname,"w"); 
  if(fp<=0) return 0;
  fwrite(fwbuf,1024,1024,fp);
  fclose(fp);
  if(retval==0) printf("Error in GetAndSaveFW!!\n");
  return retval;

}

bool UpdateFW(char *fname=NULL)
{
  bool retval=1;
  memset(fwbuf,1024*1024,00);
  printf("Opening FW file %s..\n",fname);
   FILE *fp=fopen(fname,"r");
   if(!fp) return 0;
  fread(fwbuf,1024,1024,fp);
  fclose(fp);
 printf("Programming first 64 kB into FLASH addr 0x20000..\n");
  retval=ProgramFW(0x20000,64); //put FW into safe area
  if(retval==0) return 0;
 printf("Copying 64 kB FLASH from 0x20000 to 0x0..\n");
  buf[0]=0;buf[1]=0;buf[2]=0x02;buf[3]=64;buf[4]=0; //prepare for copying 64 kB from 0x20000 to 0x0
  t->SendCMD(t->dstmac,FEB_WR_FW,0x0101,buf); //perform copy
 printf("Pause 1s to let FEB perform reset..\n");
  gSystem->Sleep(1000);
  RescanNet();
  return retval;
}


bool UpdateFPGA(char *fname=NULL)
{
  bool retval=1;
  uint8_t CRC=0;
  uint8_t CRC1=0;
uint32_t startaddr=0x80000; //put FW into second half of the FLASH
int page64;
  memset(fwbuf,1024*1024,0xFF);
  printf("Opening FW file %s..\n",fname);
   FILE *fp=fopen(fname,"r");
   if(!fp) return 0;
  fread(fwbuf,1024,1024,fp);
  fclose(fp);
 printf("Programming 512 kB into FLASH addr 0x80000..\n");
// ProgramFW(0x80000,512); //put FW into second half of the FLASH
for(int page64=0; page64<8; page64++)
{
  printf("Programming 64 kB into FLASH addr 0x%08x\n",startaddr);
  gSystem->Sleep(500); 
  buf[0]=startaddr & 0x000000ff; 
  buf[1]=(startaddr & 0x0000ff00)>>8; 
  buf[2]=(startaddr & 0x00ff0000)>>16; 
  buf[3]=64;
  buf[4]=00; 
  t->dstmac[5] = t->macs[BoardToMon][5];
  t->SendCMD(t->dstmac,FEB_WR_FW,00,buf); //initiate flash programming
  for( int i=0;i<64;i++) {
   if( t->SendCMD(t->dstmac,FEB_DATA_FW,getCRC(fwbuf+(i+64*page64)*1024,1024),fwbuf+(i+64*page64)*1024) ==0) {
      retval=0;
     // printf("Error in programming 1kB block at %08x !\n",fwbuf+(i+64*page64)*1024);
      }
  gSystem->Sleep(10); 
  }
  if( retval==0)       printf("Error in programming 64kB block at %08x !\n",startaddr);
  startaddr=startaddr+64*1024;
}
  if(retval==0) printf("Error in programmin part of UpdateFPGA!!\n");
   CRC=getCRC(fwbuf,512*1024);
   memset(fwbuf,1024*1024,00);
printf("Verifying 512 kB block at 0x80000.. Source CRC=0x%02x\n",CRC);
for(int page64=0; page64<8; page64++)    DumpFW(0x80000+page64*64*1024, 64);
   CRC1=getCRC(fwbuf,512*1024);   
printf("Programmed block CRC=0x%02x..",CRC1);
if(CRC==CRC1) printf(" OK.\n"); else {printf("ERROR !\n"); retval=0;}
  return retval;

}

void AcquireGainCalibrationData(int nevc=10000)
{
  uint32_t cmaskbefore=CHAN_MASK; //save
  int i;
  CHAN_MASK= (3 << (2*15));
  for(int sci=0; sci<16; sci++)
  {
  for(i=0; i<32;i++) //reset all chan to OFF
     {
     fChanEnaAmp[i]->SetOn(kFALSE); 
     ConfigSetBit(bufSCR,1144,633+i*15,1); 
     }
    i=sci*2;
    fChanEnaAmp[i]->SetOn(); 
    ConfigSetBit(bufSCR,1144,633+i*15,0);    
    i++;
    fChanEnaAmp[i]->SetOn(); 
    ConfigSetBit(bufSCR,1144,633+i*15,0);    
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
 // StartDAQ(nevc/100); ///for pedestal
    CHAN_MASK= (3 << (2*sci));
    printf("CHAN_MASK=%08x\n",CHAN_MASK);
  StartDAQ(nevc);

  }
  CHAN_MASK=cmaskbefore; //restore
}

void AcquirePedestalCalibrationData(int nevc=10000)
{
  uint32_t cmaskbefore=CHAN_MASK; //save
  int i;
  CHAN_MASK= (3 << (2*15));
  for(int sci=0; sci<16; sci++)
  {
  for(i=0; i<32;i++) //reset all chan to OFF
     {
     fChanEnaAmp[i]->SetOn(kFALSE); 
     ConfigSetBit(bufSCR,1144,633+i*15,1); 
     }
    i=sci*2;
    fChanEnaAmp[i]->SetOn(); 
    ConfigSetBit(bufSCR,1144,633+i*15,0);    
    i++;
    fChanEnaAmp[i]->SetOn(); 
    ConfigSetBit(bufSCR,1144,633+i*15,0);    
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
  StartDAQ(nevc); ///for pedestal
    CHAN_MASK= (3 << (2*sci));
  //  printf("CHAN_MASK=%08x\n",CHAN_MASK);
 // StartDAQ(nevc);

  }
  CHAN_MASK=cmaskbefore; //restore
}

void Calibrate(int nevcc=10000)
{
  char strnm[32];
  tr->Reset();
  AcquirePedestalCalibrationData(nevcc/5);
  sprintf(strnm,"Pedestal_SN%03d.root",t->macs[BoardToMon][5]);
  tr->SaveAs(strnm);
  tr->Reset();
  AcquireGainCalibrationData(nevcc);
  sprintf(strnm,"Gain_SN%03d.root",t->macs[BoardToMon][5]);
  tr->SaveAs(strnm); 
}

void MakeProfileTest()
{
printf("Rescanning the net..\n");
RescanNet();
Reset();
//printf("Set SiPM bias to 0V..\n");
//HVOF();
//printf("Take profile for pedestals..\n");
printf("Set SiPM bias ON..\n");
HVON();
printf("Starting DAQ for %d seconds..\n",int(fNumberEntryTME->GetNumber()));
if(RunOn==0) StartDAQ();
printf("Set SiPM bias to 0V..\n");
HVOF();
printf("Done.\n");
}

void ConfigSetFIL(uint32_t mask1)
{
 uint8_t bufFIL[256]; 
 *((uint32_t*)(&(bufFIL[0])))=mask1;
// *((uint32_t*)(&(bufFIL[4])))=mask2;
// *((uint32_t*)(&(bufFIL[8])))=mask3;
// *((uint8_t*)(&(bufFIL[12])))=majority;
 
 for(int feb=0; feb<t->nclients; feb++)
 {
  SetDstMacByIndex(feb);
  t->SendCMD(t->dstmac,FEB_WR_FIL,0x0000,bufFIL); 
 }
}
