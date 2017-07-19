
#include "RecoCTPPS/PixelLocal/interface/RPixRoadFinder.h"

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


//------------------------------------------------------------------------------------------------//

RPixRoadFinder::RPixRoadFinder(edm::ParameterSet const& parameterSet) : 
	RPixDetPatternFinder(parameterSet),
	param_(parameterSet){

	verbosity_ = param_.getUntrackedParameter<int> ("RPixVerbosity");
  roadRadius_ = param_.getParameter<double>("RPixRoadRadius");
  minRoadSize_ = param_.getParameter<int>("RPixMinRoadSize");
  maxRoadSize_ = param_.getParameter<int>("RPixMaxRoadSize");


}

//------------------------------------------------------------------------------------------------//

RPixRoadFinder::~RPixRoadFinder(){

}

//------------------------------------------------------------------------------------------------//

void RPixRoadFinder::findPattern(){

  Road temp_all_hits;
  temp_all_hits.clear();

// convert local hit sto global and push them to a vector
  for(const auto & ds_rh2 : hitVector_){
    CTPPSPixelDetId myid(ds_rh2.id);
//    uint32_t plane = myid.plane();
    for (const auto & _rh : ds_rh2.data){
      CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
      CLHEP::Hep3Vector globalV = geometry_.LocalToGlobal(ds_rh2.id,localV);
      PointInPlane pip = std::make_pair(globalV,myid);
      temp_all_hits.push_back(pip);
    }

  }
  
  Road::iterator _gh1 = temp_all_hits.begin();
  Road::iterator _gh2;

  patternVector_.clear();

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

    if(temp_road.size() > minRoadSize_ && temp_road.size() < maxRoadSize_ )patternVector_.push_back(temp_road);
   
  }
// end of algorithm

  if(verbosity_)std::cout << "+-+-+-+-+-+-    Number of pseudo tracks " << patternVector_.size() <<std::endl;

  if(patternVector_.size()>0)
    for (auto const & ptrack : patternVector_){

      if(verbosity_) std::cout << "     ptrack size = "<<ptrack.size() << std::endl;
    }

}


