
#include <FWCore/Framework/interface/Event.h>

#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelRecoAnalyzer.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"



#include <iostream>
#include <string>

#include "TFile.h"

using namespace edm;
using namespace std;

CTPPSPixelRecoAnalyzer:: CTPPSPixelRecoAnalyzer(const ParameterSet& pset) : theRPixDetTopology_(pset)
{

  label = pset.getUntrackedParameter<string>("label");  //rechitProd
  verbosity_ = pset.getParameter<int> ("Verbosity");

  file = new TFile("CTPPSPixelRecoPlots.root","RECREATE");
/*
  hOneHitperEvent = new TH2D("OneHitperEvent","One Hit per Event",30,-8.55,-8.4,20,1,1.1);
  hOneHitperEvent2 = new TH2D("OneHitperEvent2","One Hit per Event 2",30,-8.55,-8.4,20,1,1.1);
  hOneHitperEventCenter = new TH2D("OneHitperEventCenter","One Hit per Event Center",30,-0.075,0.075,20,-0.05,0.05);
  hOneHitperEvent2Center = new TH2D("OneHitperEvent2Center","Cluster Size 2",30,-0.075,0.075,20,-0.05,0.05);
  file->cd();
  hAllHits = new TH2D("AllHits","All Hits",10,1,1.1,10,-8.55,-8.4);
*/
  hDeltaRecoSim = new TH1D("DeltaRecoSim","Delta Reco Sim",100, -0.2, 0.2);
  hDeltaXRecoSim = new TH1D("DeltaXRecoSim","Delta X Reco Sim",100, -0.2, 0.2);
  hDeltaYRecoSim = new TH1D("DeltaYRecoSim","Delta Y Reco Sim",100, -0.2, 0.2);

  hDeltaRecoSimS1 = new TH1D("DeltaRecoSimS1","Delta Reco Sim size1",100, -0.2, 0.2);
  hDeltaXRecoSimS1 = new TH1D("DeltaXRecoSimS1","Delta X Reco Sim size 1",100, -0.2, 0.2);
  hDeltaYRecoSimS1 = new TH1D("DeltaYRecoSimS1","Delta Y Reco Sim size1",100, -0.2, 0.2);

  hDeltaRecoSimS2 = new TH1D("DeltaRecoSimS2","Delta Reco Sim size2",100, -0.2, 0.2);
  hDeltaXRecoSimS2 = new TH1D("DeltaXRecoSimS2","Delta X Reco Sim size 2",100, -0.2, 0.2);
  hDeltaYRecoSimS2 = new TH1D("DeltaYRecoSimS2","Delta Y Reco Sim size2",100, -0.2, 0.2);

 if(file->IsOpen()) cout<<"file open!"<<endl;
  else cout<<"*** Error in opening file ***"<<endl;



  psim_token = consumes<PSimHitContainer>( edm::InputTag("g4SimHits","CTPPSPixelHits") );
  rechit_token = consumes<edm::DetSetVector<CTPPSPixelRecHit>>( edm::InputTag(label,"") );

 
}

CTPPSPixelRecoAnalyzer::~CTPPSPixelRecoAnalyzer(){
}


void CTPPSPixelRecoAnalyzer::beginJob(){

}
void CTPPSPixelRecoAnalyzer::endJob(){

  file->cd();
/*
  hAllHits->Write();
  hOneHitperEvent->Write();
  hOneHitperEvent2->Write();
  hOneHitperEventCenter->Write();
  hOneHitperEvent2Center->Write();
*/
  hDeltaRecoSim->Write();
  hDeltaXRecoSim->Write();
  hDeltaYRecoSim->Write();

  hDeltaRecoSimS1->Write();
  hDeltaXRecoSimS1->Write();
  hDeltaYRecoSimS1->Write();

  hDeltaRecoSimS2->Write();
  hDeltaXRecoSimS2->Write();
  hDeltaYRecoSimS2->Write();

  file->Close();


  delete file;
/*
  delete hAllHits;
  delete hOneHitperEvent;
  delete hOneHitperEvent2;
  delete hOneHitperEventCenter;
  delete hOneHitperEvent2Center;
*/

  delete hDeltaRecoSim;
  delete hDeltaXRecoSim;
  delete hDeltaYRecoSim;
  delete hDeltaRecoSimS1;
  delete hDeltaXRecoSimS1;
  delete hDeltaYRecoSimS1;
  delete hDeltaRecoSimS2;
  delete hDeltaXRecoSimS2;
  delete hDeltaYRecoSimS2;
}

void  CTPPSPixelRecoAnalyzer::analyze(const Event & event, const EventSetup& eventSetup){
  if(verbosity_>0)cout << "--- Run: " << event.id().run()
		       << " Event: " << event.id().event() << endl;
  
  Handle<PSimHitContainer> simHits; 
  event.getByToken(psim_token,simHits);    

  Handle< edm::DetSetVector<CTPPSPixelRecHit> > recHits;
  event.getByToken(rechit_token, recHits);

       
  if(verbosity_>0)
    std::cout << "\n=================== Starting SimHit access" << "  ===================" << std::endl;

  if(verbosity_>1)
    std::cout << simHits->size() << std::endl;

  if(verbosity_>1)
    for(vector<PSimHit>::const_iterator hit = simHits->begin(); hit != simHits->end(); hit++){    
      LocalPoint entryP = hit->entryPoint();
      LocalPoint exitP = hit->exitPoint();
//    int partType = hit->particleType();
      LocalPoint midP ((entryP.x()+exitP.x())/2.,(entryP.y()+exitP.y())/2.);
//    float path = (exitP-entryP).mag();
//    float path_x = fabs((exitP-entryP).x());    
      if(verbosity_>1)
	if( hit->timeOfFlight() > 0){
	  cout << "DetId: " << hit->detUnitId()
	       <<" PID: "<<hit->particleType()
	       <<" TOF: "<<hit->timeOfFlight()
	       <<" Proc Type: "<<hit->processType() 
	       <<" p: " << hit->pabs()
	       <<" x = " << midP.x() << "   y = " <<midP.y() <<  "  z = " << midP.z() <<endl; 
	}
    }


  if(verbosity_>0)
    std::cout << "\n===================  Starting RecHit access" << "  ===================" << std::endl;

  vector<double> dV;
  dV.clear();
   vector<double> dVx;
  dVx.clear();
  vector<double> dVy;
  dVy.clear();
  vector<double> dV1;
  dV1.clear();
   vector<double> dVx1;
  dVx1.clear();
  vector<double> dVy1;
  dVy1.clear();
   vector<double> dV2;
  dV2.clear();
   vector<double> dVx2;
  dVx2.clear();
  vector<double> dVy2;
  dVy2.clear();

// Iterate on detector units
  edm::DetSetVector<CTPPSPixelRecHit>::const_iterator DSViter = recHits->begin();

  for( ; DSViter != recHits->end(); DSViter++) {
 
    DetId detIdObject(DSViter->detId());
    if(verbosity_>1)       std::cout << "DetId: " << DSViter->detId()<< std::endl;

// looping over rechits in a unit id
    edm::DetSet<CTPPSPixelRecHit>::const_iterator begin = (*DSViter).begin();
    edm::DetSet<CTPPSPixelRecHit>::const_iterator end = (*DSViter).end();

    if(verbosity_>1)  std::cout << "FF  "<< DSViter->detId() << std::endl;
    


    for( edm::DetSet<CTPPSPixelRecHit>::const_iterator di = begin; di != end; di++){

      if(verbosity_>1)  std::cout << "           recHit point  " << di->getPoint() << " from cluster of size " << di->clusterSize()<<std::endl ;	

      double dmin = 999;
      double dx,dy;
      for(vector<PSimHit>::const_iterator hit = simHits->begin(); hit != simHits->end(); hit++){   
	if(DSViter->detId() == hit->detUnitId()){
	  LocalPoint entryP = hit->entryPoint();
	  LocalPoint exitP = hit->exitPoint();
	  LocalPoint midP ((entryP.x()+exitP.x())/2.,(entryP.y()+exitP.y())/2.);
	  LocalPoint recP(di->getPoint().x(), di->getPoint().y(),0);
	  
	  LocalVector d = recP-midP; 
	  if(verbosity_>1)  cout << "                       "<<d.mag() <<endl;
	  
	//looking for corresponding simhit
	  if(d.mag() < dmin){
	    dmin = d.mag();
	    dx = d.x();
	    dy = d.y();
	  }
	  
	  
	}

      }// loop on simhits

      if(dmin != 999){
	dV.push_back(dmin);
	dVx.push_back(dx);
	dVy.push_back(dy);

	if(di->clusterSize()==1){
	  dV1.push_back(dmin);
	  dVx1.push_back(dx);
	  dVy1.push_back(dy);

	}else{
	  dV2.push_back(dmin);
	  dVx2.push_back(dx);
	  dVy2.push_back(dy);

	}

      }

    }

       
  }

  for( auto i : dV){
    if(verbosity_>1)
      cout << "MIN distance "<< i << endl;
    hDeltaRecoSim->Fill(i);
  }
  for( auto x : dVx){
    hDeltaXRecoSim->Fill(x);
  }
  for( auto y : dVy){
    hDeltaYRecoSim->Fill(y);
  }
  for( auto i : dV1){
    hDeltaRecoSimS1->Fill(i);
  }
  for( auto x : dVx1){
    hDeltaXRecoSimS1->Fill(x);
  }
  for( auto y : dVy1){
    hDeltaYRecoSimS1->Fill(y);
  }
  for( auto i : dV2){
    hDeltaRecoSimS2->Fill(i);
  }
  for( auto x : dVx2){
    hDeltaXRecoSimS2->Fill(x);
  }
  for( auto y : dVy2){
    hDeltaYRecoSimS2->Fill(y);
  }


}


#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CTPPSPixelRecoAnalyzer);
