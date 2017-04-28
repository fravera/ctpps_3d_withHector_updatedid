
#include <FWCore/Framework/interface/Event.h>

#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelRecoAnalyzer.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"



#include <iostream>
#include <string>

#include "TFile.h"

using namespace edm;
using namespace std;

CTPPSPixelRecoAnalyzer:: CTPPSPixelRecoAnalyzer(const ParameterSet& pset) : theRPixDetTopology_(pset)
{

  label = pset.getUntrackedParameter<string>("label");  //rechitProd
  verbosity_ = pset.getParameter<int> ("Verbosity");

  file = new TFile("CTPPSPixelDigiPlots.root","RECREATE");
  hOneHitperEvent = new TH2D("OneHitperEvent","One Hit per Event",30,-8.55,-8.4,20,1,1.1);
  hOneHitperEvent2 = new TH2D("OneHitperEvent2","One Hit per Event 2",30,-8.55,-8.4,20,1,1.1);
  hOneHitperEventCenter = new TH2D("OneHitperEventCenter","One Hit per Event Center",30,-0.075,0.075,20,-0.05,0.05);
  hOneHitperEvent2Center = new TH2D("OneHitperEvent2Center","Cluster Size 2",30,-0.075,0.075,20,-0.05,0.05);
  file->cd();
  hAllHits = new TH2D("AllHits","All Hits",10,1,1.1,10,-8.55,-8.4);


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
//cout<<"Number of analyzed event: "<<nevts<<endl;
//HitsAnalysis->Report();
  file->cd();
/*
  DigiTimeBox->Write();
  hDigis_global.Write();
  hDigis_W0.Write();
  hDigis_W1.Write();
  hDigis_W2.Write();
  hAllHits.Write();
*/
  hAllHits->Write();
  hOneHitperEvent->Write();
  hOneHitperEvent2->Write();
  hOneHitperEventCenter->Write();
  hOneHitperEvent2Center->Write();
  file->Close();


  delete file;
  delete hAllHits;
  delete hOneHitperEvent;
  delete hOneHitperEvent2;
  delete hOneHitperEventCenter;
  delete hOneHitperEvent2Center;

}

void  CTPPSPixelRecoAnalyzer::analyze(const Event & event, const EventSetup& eventSetup){
  if(verbosity_>0)cout << "--- Run: " << event.id().run()
		       << " Event: " << event.id().event() << endl;
  
  cout << "                                                            I do love Pixels     " << endl;  
  Handle<PSimHitContainer> simHits; 
  event.getByToken(psim_token,simHits);    

  Handle< edm::DetSetVector<CTPPSPixelRecHit> > recHits;
  event.getByToken(rechit_token, recHits);

       
  if(verbosity_>0)
    std::cout << "\n=================== Starting SimHit access" << "  ===================" << std::endl;

  if(verbosity_>1)
    std::cout << simHits->size() << std::endl;


  for(vector<PSimHit>::const_iterator hit = simHits->begin();
      hit != simHits->end(); hit++){    
 

    LocalPoint entryP = hit->entryPoint();
    LocalPoint exitP = hit->exitPoint();
//    int partType = hit->particleType();
    LocalPoint midP ((entryP.x()+exitP.x())/2.,(entryP.y()+exitP.y())/2.);
//    float path = (exitP-entryP).mag();
//    float path_x = fabs((exitP-entryP).x());
    
//--------------

    if(verbosity_>1)
      if( hit->timeOfFlight() > 0){
	cout << "DetId: " << hit->detUnitId()
	     <<" PID: "<<hit->particleType()
	     <<" TOF: "<<hit->timeOfFlight()
	     <<" Proc Type: "<<hit->processType() 
	     <<" p: " << hit->pabs()
	     <<" x = " << entryP.x() << "   y = " <<entryP.y() <<  "  z = " << entryP.z() <<endl;
      //     hAllHits.FillTOF(hit->timeOfFlight());


      }
  }


  if(verbosity_>0)
    std::cout << "\n===================  Starting RecHit access" << "  ===================" << std::endl;
 
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

      if(verbosity_>1)  std::cout << "           recHit point  " << di->getPoint() << std::endl ;	


    }

       
  }

}


#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CTPPSPixelRecoAnalyzer);
