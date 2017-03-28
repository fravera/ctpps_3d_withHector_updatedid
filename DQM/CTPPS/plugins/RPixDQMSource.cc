/****************************************************************************
*
* This is a part of CTPPSDQM software.
* Authors:
*   Jan Ka??par (jan.kaspar@gmail.com)
*   Rafa?? Leszko (rafal.leszko@gmail.com)
*   Vladimir Popov (vladimir.popov@cern.ch)
*
****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelIndices.h"
//#include "DataFormats/CTPPSReco/interface/RPixCluster.h"
//#include "DataFormats/CTPPSDigi/interface/CTPPSTrackerDigi.h"

#include <string>

//----------------------------------------------------------------------------------------------------
 
class RPixDQMSource: public DQMEDAnalyzer
{
 public:
   RPixDQMSource(const edm::ParameterSet& ps);
   virtual ~RPixDQMSource();
  
 protected:
   void dqmBeginRun(edm::Run const &, edm::EventSetup const &) override;
   void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
   void analyze(edm::Event const& e, edm::EventSetup const& eSetup);
   void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
   void endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
   void endRun(edm::Run const& run, edm::EventSetup const& eSetup);

 private:
   unsigned int verbosity;

//  edm::InputTag RPixDigiInputTag_;
//  edm::EDGetTokenT<edm::DetSetVector<RPixDigi>> RPixDigiInputTagToken_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelDigi>> RPixDigiToken;
  edm::EDGetTokenT< edm::DetSetVector<CTPPSPixelCluster> > tokenCluster;
//  edm::EDGetTokenT< edm::DetSetVector<RPixCluster> > tokenCluster;

 #define NRPotsMAX 6
 #define NplaneMAX 6
 const int hitMultMAX = 200;
 const int ClusMultMAX = 90;
 const int CluSizeMAX = 25;

 CTPPSPixelIndices *ctppsInd;
 int pixRowMAX = 160;  // defaultDetSizeInX, CMS Y-axis
 int pixColMAX = 156;  // defaultDetSizeInY, CMS X-axis
 int ROCSizeInX = pixRowMAX/2;  // ROC row size in pixels = 80
 int ROCSizeInY = pixColMAX/3;  // ROC col size in pixels = 52
 long int nEvents = 0;

 int pixColRow[2], multHits, multClus, cluSizeMaxData, ADCmax;//tmp

 const uint32_t row_shift=0, row_mask=0x7FF;
 const uint32_t column_shift = 11, column_mask = 0x7FF;
 const uint32_t adc_shift = 22, adc_mask = 0x3FF;

  MonitorElement *hp2HitsOcc[2];
  MonitorElement *h2HitsMultipl[2];
  MonitorElement *h2RPotActive[2];
  MonitorElement *h2PlaneActive[2];

  MonitorElement *hHitsMult[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement  *h2xyHits[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement  *hp2xyADC[2*3*NRPotsMAX][NplaneMAX];
  MonitorElement  *h2xyROCHits[2*3*NRPotsMAX*NplaneMAX][6];

  MonitorElement *h2ClusMultipl[2];
  MonitorElement *h2CluSize[2];

  int getRPindex(int arm, int station, int rp) {
	if(arm<0 || station<0 || rp<0) return(0);
	if(arm>1 || station>2 || rp>=NRPotsMAX) return(0);
//	int stn=station-1; if(stn<0) stn=0;
//	int rc = (arm*2+stn)*NRPotsMAX + rp;
	int rc = (arm*3+station)*NRPotsMAX + rp;
	return(rc);
  }

  int getPlaneIndex(int arm, int station, int rp, int plane) {
    if(plane<0 || plane>=NplaneMAX) return(0);
    int rc = getRPindex(arm, station, rp);
    if(rc == 0 && (arm !=0 || station !=0 || rp !=0)) return(0);
    return(rc*NplaneMAX + plane);
  }

  void Scale2DF(TH2D *h1, TH2F *h2);
  int prIndex(int rp, int plane) {return(rp*NplaneMAX + plane);}
  int getDet(int id) { return (id>>28)&0xF; }
  int getPixPlane(int id) { return ((id>>16)&0x7); }
  int getSubdet(int id) { return ((id>>25)&0x7); }
  int getHitRow(uint32_t pkdata) {return (pkdata >> row_shift) & row_mask;}
 int getHitColumn(uint32_t pkdata) const  
	{return (pkdata >> column_shift) & column_mask;}
 unsigned short getHitADC(uint32_t pkdata) const 
	{return (pkdata >> adc_shift) & adc_mask;}

};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

RPixDQMSource::RPixDQMSource(const edm::ParameterSet& ps) :
  verbosity(ps.getUntrackedParameter<unsigned int>("verbosity", 0))
{
 RPixDigiToken = consumes<DetSetVector<CTPPSPixelDigi> >(ps.getParameter<edm::InputTag>("tagRPixDigi"));
 tokenCluster= 
consumes<DetSetVector<CTPPSPixelCluster>>(ps.getParameter<edm::InputTag>("tagRPixCluster"));
//consumes<DetSetVector<RPixCluster>>(ps.getParameter<edm::InputTag>("tagRPixCluster"));

}

//----------------------------------------------------------------------------------------------------

RPixDQMSource::~RPixDQMSource()
{
}

//----------------------------------------------------------------------------------------------------

void RPixDQMSource::dqmBeginRun(edm::Run const &run, edm::EventSetup const &)
{
 if(verbosity) printf("RPixDQMSource::dqmBeginRun(): RunID=%d\n",run.run());
 nEvents = 0;
 for(int i=0; i<2; i++) pixColRow[i] = -1;
 multHits = multClus = cluSizeMaxData = ADCmax = -1;

/*
 std::vector<edm::DetSet<CTPPSPixelDigi> > theDigiVector;
 DetSet<CTPPSPixelDigi> *p = 
	new DetSet<CTPPSPixelDigi>(&theDigiVector);
 int row_mask2 = p->row_mask;
*/
}

//----------------------------------------------------------------------------------------------------

void RPixDQMSource::bookHistograms(DQMStore::IBooker &ibooker, edm::Run const &, edm::EventSetup const &)
{
//  printf("CTPPSDetId::sdTrackingPixel=%d CTPPSDetId::maxRP=%d\n",
//	CTPPSDetId::sdTrackingPixel,CTPPSDetId::maxRP);

  ctppsInd = new CTPPSPixelIndices();
  pixRowMAX = ctppsInd->getDefaultRowDetSize();
  pixColMAX = ctppsInd->getDefaultColDetSize();

  ROCSizeInX = pixRowMAX/2;  // ROC row size in pixels = 80
  ROCSizeInY = pixColMAX/3;
  if(verbosity) printf("bookHistograms: sizeX/Y=%d/%d ROCSizeInX/Y=%d/%d\n",
	pixColMAX,pixRowMAX,ROCSizeInX,ROCSizeInY);

  ibooker.cd();
  ibooker.setCurrentFolder("CTPPS/RPix");
  char s[50];

 for(int arm=0; arm<2; arm++) {
   sprintf(s,"CTPPS/RPix/Arm_%1d",arm);
   ibooker.setCurrentFolder(s);
   sprintf(s,"RPots activity (arm=%d)",arm);
  h2RPotActive[arm] = ibooker.book2DD(s,s, NRPotsMAX, 0,NRPotsMAX, 3,0,3);
  TH2D *h6 = h2RPotActive[arm]->getTH2D();
   h6->SetOption("colz");
   h6->GetXaxis()->SetTitle("RPot #");
   h6->GetYaxis()->SetTitle("Station #");

 for(int stn=0; stn<3; stn++) {
   CTPPSDetId stId(CTPPSDetId::sdTrackingPixel,arm, stn);
   string title;
   CTPPSDetId(stId).stationName(title, CTPPSDetId::nShort);
   if(verbosity) { printf("arm/stn = %d/%d stId=0x%x ",arm,stn,int(stId));
	cout<<"title: "<<title<<endl;}

   sprintf(s,"CTPPS/RPix/Arm_%1d/Station_",arm);
   string s2 = string(s) + string(title);
   if(verbosity) cout<<"s+title: "<<s2<<endl;
   ibooker.setCurrentFolder(s2);

   sprintf(s,"(arm=%d Station_",arm);
   string str = string(s) + title;

   sprintf(s,"planes activity");
   string st = string(s) + str;
  h2PlaneActive[arm] = ibooker.book2DD(st,st,NplaneMAX,0,NplaneMAX, NRPotsMAX, 0,NRPotsMAX);
  TH2D *h = h2PlaneActive[arm]->getTH2D();
   h->SetOption("colz");
   h->GetXaxis()->SetTitle("Plane #");
   h->GetYaxis()->SetTitle("RPot #");

   sprintf(s,"hit average multiplicity in planes");
   st = string(s) + str;
   hp2HitsOcc[arm]= ibooker.bookProfile2D(st,st,NplaneMAX, 0, NplaneMAX,
	int(NRPotsMAX),0,int(NRPotsMAX),0,20000); //MAXSPAN^2
   TProfile2D *h2 = hp2HitsOcc[arm]->getTProfile2D();
   h2->SetOption("textcolz");
   h2->GetXaxis()->SetTitle("Plane #");
   h2->GetYaxis()->SetTitle("RPot #");

   int bxmax = (int)NplaneMAX*int(NRPotsMAX);

   sprintf(s,"hit multiplicity in planes");
   st = string(s) + str;
 h2HitsMultipl[arm]= ibooker.book2DD(st,st,bxmax,0,bxmax,hitMultMAX,0,hitMultMAX);
   TH2D *h3 = h2HitsMultipl[arm]->getTH2D();
   h3->SetOption("colz");
   h3->GetXaxis()->SetTitle("PlaneIndex(=rpot*PlaneMAX + plane)");
   h3->GetYaxis()->SetTitle("multiplicity");

   sprintf(s,"cluster multiplicity in planes");
   st = string(s) + str;
   h2ClusMultipl[arm]= ibooker.book2DD(st,st,bxmax,0,bxmax, ClusMultMAX,0,ClusMultMAX);
   TH2D *h4 = h2ClusMultipl[arm]->getTH2D();
   h4->SetOption("colz");
   h4->GetXaxis()->SetTitle("PlaneIndex(=rpot*PlaneMAX + plane)");
   h4->GetYaxis()->SetTitle("multiplicity");

   sprintf(s,"cluster size in planes");
   st = string(s) + str;
  h2CluSize[arm]= ibooker.book2D(st,st,bxmax,0,bxmax, CluSizeMAX,0,CluSizeMAX);
   TH2F *h5 = h2CluSize[arm]->getTH2F();
   h5->SetOption("colz");
   h5->GetXaxis()->SetTitle("PlaneIndex(=rpot*PlaneMAX + plane)");
   h5->GetYaxis()->SetTitle("Cluster size");

//--------- Hits ---
   int pixBinW = 4;
   for(int rp=0; rp<NRPotsMAX; rp++) {
    sprintf(s,"/RP_%d",rp);
    string s3 = s2 + string(s);
    ibooker.setCurrentFolder(s3);
    int nbins = pixRowMAX/pixBinW;
//    int indexP = getRPindex(arm,stn+1,rp); // to be consistent with station numbering: 2==220
    int indexP = getRPindex(arm,stn,rp);

     for(int p=0; p<NplaneMAX; p++) {
      char st1[80], st2[80];
      sprintf(st1,"(arm=%d Station_",arm);
      sprintf(st2," rp=%d plane=%d)",rp,p);
      string str = string(st1) + title +string(st2);
       sprintf(st1,"hits position");
       string st = string(st1) + str;
h2xyHits[indexP][p]=ibooker.book2DD(st,st,nbins,0,pixRowMAX,nbins,0,pixRowMAX);
     TH2D *h2 = h2xyHits[indexP][p]->getTH2D();
     h2->SetOption("colz");
     h2->GetXaxis()->SetTitle("pix col");
     h2->GetYaxis()->SetTitle("pix row");

   sprintf(st1,"adc average value");
   st = string(st1) + str;
hp2xyADC[indexP][p]=ibooker.bookProfile2D(st,st,nbins,0,pixRowMAX,nbins,0,pixRowMAX,100,0,1.e10,"");
     TProfile2D *hp2 = hp2xyADC[indexP][p]->getTProfile2D();
     hp2->SetOption("colz");
     hp2->GetXaxis()->SetTitle("pix col");
     hp2->GetYaxis()->SetTitle("pix row");

   sprintf(st1,"hits multiplicity");
   st = string(st1) + str;
     hHitsMult[indexP][p]=ibooker.book1DD(st,st,hitMultMAX,0,hitMultMAX);
     TH1D *h = hHitsMult[indexP][p]->getTH1D();
     h->GetXaxis()->SetTitle("number of hits");
     h->GetYaxis()->SetTitle("N / 1 hit");
    }// end of for(int p=0; p<NplaneMAX;..

    for(int p=0; p<NplaneMAX; p++) {
      sprintf(s,"/ROCs/plane_%d",p);
      string s4 = s3 + string(s);
     ibooker.setCurrentFolder(s4);
//     int index = getPlaneIndex(arm,stn+1,rp,p);
     int index = getPlaneIndex(arm,stn,rp,p);

     for(int roc=0; roc<6; roc++) {
      char st1[80], st2[80];
      sprintf(st1,"hits in ROC(arm=%d Station_",arm);
      sprintf(st2," rp=%d plane=%d roc=%d)",rp,p,roc);
      string st = string(st1) + title +string(st2);

       h2xyROCHits[index][roc]=
	ibooker.book2DD(st,st,ROCSizeInX,0,ROCSizeInX,ROCSizeInY,0,ROCSizeInY);
//	ibooker.book2DD(st,st,ROCSizeInX,0,ROCSizeInX,ROCSizeInX,0,ROCSizeInX);
      TH2D *h2 = h2xyROCHits[index][roc]->getTH2D();
      h2->SetOption("colz");
      h2->GetYaxis()->SetTitle("ROC col");
      h2->GetXaxis()->SetTitle("ROC row");
     }
    }
   } // end for(int rp=0; rp<NRPotsMAX;...

  } // end of for(int stn=0; stn<2; stn++
  } //end of for(int arm=0; arm<2;...

  if(verbosity) printf("bookHistograms: end\n");
 return;
}

//----------------------------------------------------------------------------------------------------

void RPixDQMSource::beginLuminosityBlock(edm::LuminosityBlock const& lumiSeg, 
                                            edm::EventSetup const& context) 
{
}

//----------------------------------------------------------------------------------------------------

void RPixDQMSource::analyze(edm::Event const& event, edm::EventSetup const& eventSetup)
{
  ++nEvents;
  if(verbosity>2) printf("irun=%d\n",event.id().run());

/*
  // event setup data
  ESHandle<TotemRPGeometry> geometry;
  eventSetup.get<VeryForwardRealGeometryRecord>().get(geometry);
*/
//  Handle< DetSetVector<RPixDigi> > pixDigi;
  Handle< DetSetVector<CTPPSPixelDigi> > pixDigi;
  event.getByToken(RPixDigiToken, pixDigi);

//  Handle< DetSetVector<RPixCluster> > Clus;
  Handle< DetSetVector<CTPPSPixelCluster> > Clus;
  event.getByToken(tokenCluster, Clus);

  // check validity
  bool valid = true;
  valid &= pixDigi.isValid();
  valid &= Clus.isValid();
//  valid &= hits.isValid();

 if(!valid && verbosity>2) 
printf("pixDigi/cluster.isValid()=%d/%d\n",pixDigi.isValid(),Clus.isValid());

  int ind = 0;
  for(const auto &ds_digi : *pixDigi)
  {
   int idet = getDet(ds_digi.id);
   if(idet != DetId::VeryForward) {
     if(verbosity>1) printf("not CTPPS: ds_digi.id=0x%x\n",ds_digi.id);
     continue;
   }
   int subdet = getSubdet(ds_digi.id);

   int plane = getPixPlane(ds_digi.id);
   int arm = CTPPSDetId(ds_digi.id).arm()&0x1;
   int station = CTPPSDetId(ds_digi.id).station();
   int rpot = CTPPSDetId(ds_digi.id).rp();

  string title;
  CTPPSDetId(ds_digi.id).stationName(title, CTPPSDetId::nShort);
//  cout<<"title: "<<title<<endl;

  if(verbosity>1) printf(
 "[%d]RpixAnalyser::digi : 0x%x(idet/subdet: 0x%x / 0x%x) arm/station=%d/%d rp/plane=%d/%d\n",
        ind++,ds_digi.id,idet,subdet,arm,station,rpot,plane);

   hp2HitsOcc[arm]->Fill(plane,rpot,(int)ds_digi.data.size());
   h2HitsMultipl[arm]->Fill(prIndex(rpot,plane),ds_digi.data.size());
   int index = getRPindex(arm,station,rpot);
   hHitsMult[index][plane]->Fill(ds_digi.data.size());

   if(int(ds_digi.data.size()) > multHits) multHits = ds_digi.data.size();
   h2RPotActive[arm]->Fill(rpot,station);
   h2PlaneActive[arm]->Fill(plane,rpot);

 if(verbosity>1)
   printf("ds_digi.data.size()=%d\n",(int)ds_digi.data.size());
     int rocHistIndex = getPlaneIndex(arm,station,rpot,plane);

for(DetSet<CTPPSPixelDigi>::const_iterator dit = ds_digi.begin();
	 dit != ds_digi.end(); ++dit) {
     int pkdata = dit->packedData();
     int row = getHitRow(dit->packedData());
     int col = getHitColumn(dit->packedData());
     int adc = getHitADC(dit->packedData());

     h2xyHits[index][plane]->Fill(col,row);
     hp2xyADC[index][plane]->Fill(col,row,adc);

     if(pixColRow[0] < col) pixColRow[0] = col;
     if(pixColRow[1] < row) pixColRow[1] = row;
     if(ADCmax < adc) ADCmax = adc;

     int colROC, rowROC;
     int trocId;
   if(!ctppsInd->transformToROC(col,row, trocId, colROC, rowROC)) {
     h2xyROCHits[rocHistIndex][trocId]->Fill(rowROC,colROC);
   }

     if(verbosity>2)
     printf("packedData = 0x%x row/column=%d/%d adc=%d rocId=%d RocHistIndex=%d\n",
	pkdata,row,col,adc,trocId,rocHistIndex);
   }
 } // for(const auto &ds_digi : *pixDigi)

  ind=0;
 if(Clus.isValid())
  for(const auto &ds : *Clus)
  {
   int idet = getDet(ds.id);
   int subdet = getSubdet(ds.id);
   int station = CTPPSDetId(ds.id).station();

   int plane = getPixPlane(ds.id);
   int arm = CTPPSDetId(ds.id).arm()&0x1;
   int rpot = CTPPSDetId(ds.id).rp();
  if(verbosity>2) printf(
 "[%d]RpixAnalyser::Cluster : 0x%x(0x%x / 0x%x) arm/station=%d/%d rp/plane=%d/%d \n",
        ind++,ds.id,idet,subdet,arm,station,rpot,plane);

   h2ClusMultipl[arm]->Fill(prIndex(rpot,plane),ds.data.size());
   if(multClus < (int)ds.data.size()) multClus = ds.data.size();
 if(verbosity>1)
   printf("ds.data.size()=%d:-----\n",(int)ds.data.size());
   for (const auto &p : ds) {
//     int isize = p.size();
    h2CluSize[arm]->Fill(prIndex(rpot,plane),p.size());
   if(cluSizeMaxData < p.size()) cluSizeMaxData = p.size();
   }
  } // end 'for(const auto &ds : *Clus)'

  if((nEvents % 50)) return;
  if(verbosity) printf("RPixAnalyser::analyze event= %d\n",(int)nEvents);
}

//--------------------------------------------------------------
void RPixDQMSource::endLuminosityBlock(edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& eSetup) 
{
}

//----------------------------------------------------------------------------------------------------
void RPixDQMSource::endRun(edm::Run const& run, edm::EventSetup const& eSetup)
{
 if(!verbosity) return; 
  printf("end of Run %d: %d events\n",run.run(),(int)nEvents);
  printf("column/row max: %d / %d\n",pixColRow[0],pixColRow[1]);
  printf("mult Hits/Clus: %d / %d\n",multHits,multClus);
  printf("cluSizeMaxData=%d adcMax= %d\n",cluSizeMaxData,ADCmax);
  printf("---------- end of Run -------\n");
}


//----------------------------------------------------------------------------------------------------

//DEFINE_FWK_MODULE(TotemRPDQMSource);
DEFINE_FWK_MODULE(RPixDQMSource);

