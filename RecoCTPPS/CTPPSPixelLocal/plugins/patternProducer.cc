
#include "RecoCTPPS/CTPPSPixelLocal/interface/patternProducer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//needed for the geometry:
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include <vector>
#include <memory>
#include <string>
#include <iostream>


patternProducer::patternProducer(const edm::ParameterSet& conf) :
  param_(conf)
{
  
  src_ = conf.getUntrackedParameter<std::string>("label");
  verbosity_ = conf.getParameter<int> ("RPixVerbosity");
	 
  tokenCTPPSPixelRecHit_ = consumes<edm::DetSetVector<CTPPSPixelRecHit> >(edm::InputTag(src_));
 
//  produces<edm::DetSetVector<CTPPSPixelRecHit> > ();
}

patternProducer::~patternProducer() {

}


// ------------ method called to produce the data  ------------
void patternProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   
  std::cout << "  ****************************************************** " << std::endl;
  std::cout << "  ****        Inside patternHitProducer        ***** " << std::endl;
  std::cout << "  ****************************************************** " << std::endl;

// Step A: get inputs

	edm::Handle<edm::DetSetVector<CTPPSPixelRecHit> > rpRh;
	iEvent.getByToken(tokenCTPPSPixelRecHit_, rpRh);

 // get geometry
 //----------------------------------

	edm::ESHandle<TotemRPGeometry> geometry;
//	iSetup.get<VeryForwardMisalignedGeometryRecord>().get(geometry);
	iSetup.get<VeryForwardRealGeometryRecord>().get(geometry);

	geometryWatcher.check(iSetup);

// dry checks to be removed
/*
	CTPPSPixelDetId iid(0,2,3,1);
	std::cout << iid << std::endl;
	std::cout << iid.rawId() << std::endl;
	unsigned int rpId = 2023292928;

	CLHEP::Hep3Vector localV(-4.43825,2.05224,0.115);
	CLHEP::Hep3Vector globalV = geometry->LocalToGlobal(rpId,localV);


	std::cout << "id: "<< rpId <<"   local " << localV <<"   to global "<<globalV<< std::endl;
*/
//---------------------------------------------



//	edm::DetSetVector<CTPPSPixelRecHit>  output;

// run reconstruction
	if (rpRh->size()){

	  run(*rpRh, *geometry);
	}

	const edm::DetSetVector<CTPPSPixelRecHit> &rpRhh = *rpRh;
//==============
	unsigned int idSS = 0;
	for(const auto & ds_rh : rpRhh){
	  idSS++;
	  if(verbosity_)std::cout << "ID :  " << ds_rh.id << std::endl;
	}
	if(idSS >=5 ){

	  for(const auto & ds_rh2 : rpRhh){
	    for (const auto & _rh : ds_rh2.data){

	      CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
	      CLHEP::Hep3Vector globalV = geometry->LocalToGlobal(ds_rh2.id,localV);
	      if(verbosity_)std::cout << "ID : " << ds_rh2.id << " hit  " << _rh.getPoint().x()<<" "<<_rh.getPoint().y()<<" " <<_rh.getPoint().z()<< "   "<< globalV.x() << " " << globalV.y() <<" " <<std::setprecision(6) << globalV.z() <<std::endl;

	    }
	  }
	}
	
//==============       
//	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelRecHit> >(output));

}

void patternProducer::run(const edm::DetSetVector<CTPPSPixelRecHit> &input, const TotemRPGeometry &geometry){

  std::vector<CLHEP::Hep3Vector> temp_global_hits;
  temp_global_hits.clear();

// convert local hit sto global and push them to a vector
  for(const auto & ds_rh2 : input){
    for (const auto & _rh : ds_rh2.data){

      CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
      CLHEP::Hep3Vector globalV = geometry.LocalToGlobal(ds_rh2.id,localV);

      temp_global_hits.push_back(globalV);
    }

  }
  std::vector<std::vector<CLHEP::Hep3Vector> > pseudo_tracks;
  
  std::vector<CLHEP::Hep3Vector>::iterator _gh1 = temp_global_hits.begin();
  std::vector<CLHEP::Hep3Vector>::iterator _gh2;

  pseudo_tracks.clear();

  while(temp_global_hits.size() > 3){
//look for points near wrt each other
  while( _gh1 != temp_global_hits.end()){
    std::vector<CLHEP::Hep3Vector> temp_pseudo_track;
    _gh2 = _gh1;
    CLHEP::Hep3Vector currPoint = *_gh1;
    while( _gh2 != temp_global_hits.end()){
      
      CLHEP::Hep3Vector subtraction = currPoint - *_gh2;
      std::cout << " Subtraction " << *_gh1 << " - " << *_gh2 << " " << subtraction.mag() << std::endl;
      if(subtraction.mag() < 1) {  /// 1mm
	temp_pseudo_track.push_back(*_gh2);
	temp_global_hits.erase(_gh2);
      }else{
	++_gh2;
      }
std::cout << " SIZE " << temp_global_hits.size() <<std::endl;
    }
    if(temp_global_hits.size()>0)
      temp_global_hits.erase(_gh1);
    if(temp_pseudo_track.size() >= 4)pseudo_tracks.push_back(temp_pseudo_track);
   
  }

  }

  std::cout << "+-+-+-+-+-+-    Number of pseudo tracks " << pseudo_tracks.size() <<std::endl;

  if(pseudo_tracks.size()>0)
    for (auto const & ptrack : pseudo_tracks){

      std::cout << "     ptrack size = "<<ptrack.size() << std::endl;
    }

}

/*
void CTPPSPixelRecHitProducer::run(const edm::DetSetVector<CTPPSPixelCluster> &input, edm::DetSetVector<CTPPSPixelRecHit> &output){

  for (const auto &ds_cluster : input)
    {
      edm::DetSet<CTPPSPixelRecHit> &ds_rechit = output.find_or_insert(ds_cluster.id);



         cluster2hit_.buildHits(ds_cluster.id, ds_cluster.data, ds_rechit.data);



	

      unsigned int rechitN=0;
      for(std::vector<CTPPSPixelRecHit>::iterator iit = ds_rechit.data.begin(); iit != ds_rechit.data.end(); iit++){
	++rechitN;
	if(verbosity_)	std::cout << "Rechit " << rechitN <<" x " << (*iit).getPoint().x()<< " y " << (*iit).getPoint().y()<< " z " <<  (*iit).getPoint().z() << std::endl;





      }

      if(verbosity_)	std::cout << "Rechit Number = " << rechitN << std::endl; 





    }
}
*/
DEFINE_FWK_MODULE( patternProducer);
