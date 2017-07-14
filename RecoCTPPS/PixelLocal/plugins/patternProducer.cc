
#include "RecoCTPPS/PixelLocal/interface/patternProducer.h"

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
  
  src_ = conf.getParameter<std::string>("label");
  verbosity_ = conf.getUntrackedParameter<int> ("RPixVerbosity");
  roadRadius_ = conf.getParameter<double>("RPixRoadRadius");
  minRoadSize_ = conf.getParameter<int>("RPixMinRoadSize");
  maxRoadSize_ = conf.getParameter<int>("RPixMaxRoadSize");
 
  tokenCTPPSPixelRecHit_ = consumes<edm::DetSetVector<CTPPSPixelRecHit> >(edm::InputTag(src_));
 
//  produces<edm::DetSetVector<CTPPSPixelRecHit> > ();
}

patternProducer::~patternProducer() {

}

void patternProducer::fillDescriptions(edm::ConfigurationDescriptions & descriptions){
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("RPixVerbosity",0);
  desc.add<std::string>("label", "ctppsPixelRecHits");
  desc.add<double>("RPixRoadRadius",1.0);
  desc.add<int>("RPixMinRoadSize",3);
  desc.add<int>("RPixMaxRoadSize",20);
  descriptions.add("patternProd", desc);
}

// ------------ method called to produce the data  ------------
void patternProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   
  if(verbosity_)std::cout << "  ****************************************************** " << std::endl;
  if(verbosity_)std::cout << "  ****        Inside patternHitProducer        ***** " << std::endl;
  if(verbosity_)std::cout << "  ****************************************************** " << std::endl;

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
  std::vector<Road> roads;
  if (rpRh->size()){
    run(*rpRh, *geometry, roads);
  }


//==============
// checking results (to be removed)
  const edm::DetSetVector<CTPPSPixelRecHit> &rpRhh = *rpRh;
  unsigned int idSS = 0;
  for(const auto & ds_rh : rpRhh){
    idSS++;
    CTPPSPixelDetId myid(ds_rh.id);
    if(verbosity_>1)std::cout << "ID :  " << ds_rh.id << "  plane " << myid.plane()<<std::endl;
  }
  if(idSS >=5 ){

    for(const auto & ds_rh2 : rpRhh){
      for (const auto & _rh : ds_rh2.data){

	CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
	CLHEP::Hep3Vector globalV = geometry->LocalToGlobal(ds_rh2.id,localV);
	if(verbosity_>1)std::cout << "ID : " << ds_rh2.id << " hit  " << _rh.getPoint().x()<<" "<<_rh.getPoint().y()<<" " <<_rh.getPoint().z()<< "   "<< globalV.x() << " " << globalV.y() <<" " <<std::setprecision(6) << globalV.z() <<std::endl;

      }
    }
  }
  unsigned int ii = 0;
  for( const auto & _rr : roads ){
    ++ii;
    if(verbosity_>1)std::cout << " road #" << ii << " size : " <<  _rr.size() << std::endl;
  }
//==============       
//	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelRecHit> >(output));

}

void patternProducer::run(const edm::DetSetVector<CTPPSPixelRecHit> &input, const TotemRPGeometry &geometry,   std::vector<Road> &roads){

  Road temp_all_hits;
  temp_all_hits.clear();

// convert local hit sto global and push them to a vector
  for(const auto & ds_rh2 : input){
    CTPPSPixelDetId myid(ds_rh2.id);
//    uint32_t plane = myid.plane();
    for (const auto & _rh : ds_rh2.data){
      CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
      CLHEP::Hep3Vector globalV = geometry.LocalToGlobal(ds_rh2.id,localV);
      PointInPlane pip = std::make_pair(globalV,myid);
      temp_all_hits.push_back(pip);
    }

  }
  
  Road::iterator _gh1 = temp_all_hits.begin();
  Road::iterator _gh2;

  roads.clear();

//look for points near wrt each other
// starting algorithm
  while( _gh1 != temp_all_hits.end() && temp_all_hits.size() > minRoadSize_){
    Road temp_road;
  
    _gh2 = _gh1;

    CLHEP::Hep3Vector currPoint = _gh1->first;
    CTPPSPixelDetId currDet = _gh1->second;
    if(verbosity_>1)  std::cout << " current point " << currPoint << std::endl;
    while( _gh2 != temp_all_hits.end()){
      bool same_pot = false;
//      if((currPoint.z() > 0 && _gh2->first.z() > 0) || (currPoint.z() < 0 && _gh2->first.z() < 0)) same_arm = true;
      if(    currDet.arm() == _gh2->second.arm() && currDet.station() == _gh2->second.station() && currDet.rp() == _gh2->second.rp() )same_pot = true;
      CLHEP::Hep3Vector subtraction = currPoint - _gh2->first;
      if(verbosity_>1) std::cout << "             Subtraction " << currPoint << " - " << _gh2->first << " " << subtraction.perp() << std::endl;
      if(subtraction.perp() < roadRadius_ && same_pot) {  /// 1mm
	temp_road.push_back(*_gh2);
	temp_all_hits.erase(_gh2);
      }else{
	++_gh2;
      }
      if(verbosity_>1)std::cout << " SIZE " << temp_all_hits.size() <<std::endl;
    }

    if(temp_road.size() > minRoadSize_ && temp_road.size() < maxRoadSize_ )roads.push_back(temp_road);
   
  }
// end of algorithm

  if(verbosity_)std::cout << "+-+-+-+-+-+-    Number of pseudo tracks " << roads.size() <<std::endl;

  if(roads.size()>0)
    for (auto const & ptrack : roads){

      if(verbosity_) std::cout << "     ptrack size = "<<ptrack.size() << std::endl;
    }

}


DEFINE_FWK_MODULE( patternProducer);
