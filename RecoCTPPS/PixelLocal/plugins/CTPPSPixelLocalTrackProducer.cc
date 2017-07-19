
#include "RecoCTPPS/PixelLocal/interface/CTPPSPixelLocalTrackProducer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include "RecoCTPPS/PixelLocal/interface/testPatternAlgorithm.h"
#include "RecoCTPPS/PixelLocal/interface/testTrackingAlgorithm.h"
#include "RecoCTPPS/PixelLocal/interface/RPixRoadFinder.h"

#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"


//------------------------------------------------------------------------------------------------//

CTPPSPixelLocalTrackProducer::CTPPSPixelLocalTrackProducer(const edm::ParameterSet& parameterSet) :
  parameterSet_(parameterSet){

  inputTag_            = parameterSet_.getParameter<std::string>  ("label"                  );
  verbosity_           = parameterSet_.getUntrackedParameter<int> ("RPixVerbosity"          );
  maxHitPerRomanPot_   = parameterSet_.getUntrackedParameter<int> ("RPixMaxHitPerRomanPot"  );
  maxHitPerPlane_      = parameterSet_.getUntrackedParameter<int> ("RPixMaxHitPerPlane"     );
  maxTrackPerRomanPot_ = parameterSet_.getUntrackedParameter<int> ("RPixMaxTrackPerRomanPot");
  maxTrackPerPattern_  = parameterSet_.getUntrackedParameter<int> ("RPixMaxTrackPerPattern" );

  std::string patterFinderAlgorithm = parameterSet_.getParameter<std::string>("RPixPatterFinderAlgorithm");
  std::string trackFitterAlgorithm  = parameterSet_.getParameter<std::string>("RPixTrackFinderAlgorithm" );

  // patter algorithm selector
  if(patterFinderAlgorithm == "testPatternAlgorithm"){
    patternFinder_ = new testPatternAlgorithm(parameterSet_);
  }
  else if(patterFinderAlgorithm == "RPixRoadFinder"){
   patternFinder_ = new RPixRoadFinder(parameterSet_);
  }
  else{
    throw cms::Exception("CTPPSPixelLocalTrackProducer") << "Pattern finder algorithm" << patterFinderAlgorithm << " does not exist";
  }

  // tracking algorithm selector
  if(trackFitterAlgorithm == "testTrackingAlgorithm"){
    trackFinder_ = new testTrackingAlgorithm(parameterSet_);
  }
  else{
    throw cms::Exception("CTPPSPixelLocalTrackProducer") << "Tracking fitter algorithm" << trackFitterAlgorithm << " does not exist";
  }
  
  tokenCTPPSPixelRecHit_ = consumes<edm::DetSetVector<CTPPSPixelRecHit> >(edm::InputTag(inputTag_));
 
  produces<edm::DetSetVector<CTPPSPixelLocalTrack> > ();

}

//------------------------------------------------------------------------------------------------//

CTPPSPixelLocalTrackProducer::~CTPPSPixelLocalTrackProducer(){
  delete patternFinder_;
  delete trackFinder_ ;
}

//------------------------------------------------------------------------------------------------//

void CTPPSPixelLocalTrackProducer::fillDescriptions(edm::ConfigurationDescriptions & descriptions){
  edm::ParameterSetDescription desc;

  desc.add<std::string> ("label"                    , "ctppsPixelRecHits"    );
  desc.add<std::string> ("RPixPatterFinderAlgorithm", "RPixRoadFinder"       );
  desc.add<std::string> ("RPixTrackFinderAlgorithm" , "testTrackingAlgorithm");
  desc.addUntracked<int>("RPixVerbosity"            , 0                      );
  desc.addUntracked<int>("RPixMaxHitPerPlane"       , 20                     );
  desc.addUntracked<int>("RPixMaxHitPerRomanPot"    , 60                     );
  desc.addUntracked<int>("RPixMaxTrackPerRomanPot"  , 10                     );
  desc.addUntracked<int>("RPixMaxTrackPerPattern"   , 5                      );
  desc.add<double>      ("RPixRoadRadius"           , 1.0                    );
  desc.add<int>         ("RPixMinRoadSize"          , 3                      );
  desc.add<int>         ("RPixMaxRoadSize"          , 20                     );

  descriptions.add("ctppsPixelTracks", desc);
}

//------------------------------------------------------------------------------------------------//

void CTPPSPixelLocalTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  // Step A: get inputs
  
  edm::Handle<edm::DetSetVector<CTPPSPixelRecHit> > recHits;
  iEvent.getByToken(tokenCTPPSPixelRecHit_, recHits);
  const edm::DetSetVector<CTPPSPixelRecHit> &recHitVectorOrig = *recHits;
  edm::DetSetVector<CTPPSPixelRecHit> recHitVector = recHitVectorOrig;

  std::vector<CTPPSDetId> listOfPotWithHighOccupancyPlanes; 

  std::map<CTPPSDetId, uint32_t> mapHitPerPot;

  //for(edm::DetSetVector<CTPPSPixelRecHit>::iterator it=recHitVector.begin(); it!=recHitVector.end(); ++it){
  for(const auto & recHitSet : recHitVector){
    
    if(verbosity_>2) cout<<"Track found in plane = "<<recHitSet.detId()<< " number = " << recHitSet.size() <<std::endl;
    CTPPSDetId tmpRomanPotId = CTPPSDetId(recHitSet.detId()).getRPId();
    uint32_t hitOnPlane = recHitSet.size();

    //Get the number of hits per pot
    if(mapHitPerPot.find(tmpRomanPotId)==mapHitPerPot.end()){
      mapHitPerPot[tmpRomanPotId] = hitOnPlane;
    }
    else mapHitPerPot[tmpRomanPotId]+=hitOnPlane;

    //check is the plane occupancy is too high and save the corresponding pot
    if(maxHitPerPlane_>=0 && hitOnPlane>(uint32_t)maxHitPerPlane_){
      if(verbosity_>2) std::cout<<" ---> To many tracks in the plane, pot will be excluded from tracking cleared"<<std::endl;
      listOfPotWithHighOccupancyPlanes.push_back(tmpRomanPotId);
    }
  }

  //remove rechit for pot with too many hits of containing planes with too many hits
  //for(edm::DetSetVector<CTPPSPixelRecHit>::iterator it=recHitVector.begin(); it!=recHitVector.end(); ++it){
  for(const auto & recHitSet : recHitVector){
    CTPPSDetId tmpRomanPotId = CTPPSDetId(recHitSet.detId()).getRPId();

    if((maxHitPerRomanPot_>=0 && mapHitPerPot[tmpRomanPotId]>(uint32_t)maxHitPerRomanPot_) || 
      find (listOfPotWithHighOccupancyPlanes.begin(), listOfPotWithHighOccupancyPlanes.end(), tmpRomanPotId) != listOfPotWithHighOccupancyPlanes.end() ){

      edm::DetSet<CTPPSPixelRecHit> &tmpDetSet = recHitVector[tmpRomanPotId];
      tmpDetSet.clear();      

    }
  }

  // get geometry
  edm::ESHandle<TotemRPGeometry> geometryHandler;
  iSetup.get<VeryForwardRealGeometryRecord>().get(geometryHandler);
  const TotemRPGeometry &geometry = *geometryHandler;

  geometryWatcher_.check(iSetup);

  //std::cout<<"This is the size of CTPPSPixelLocalTrack"<< sizeof(CTPPSPixelLocalTrack)<<std::endl;

  edm::DetSetVector<CTPPSPixelLocalTrack> foundTracks;

  // Pattern finder

  patternFinder_->clear();
  patternFinder_->setHits(recHitVector);
  patternFinder_->setGeometry(geometry);
  patternFinder_->findPattern();
  std::vector<std::vector<std::pair<CLHEP::Hep3Vector, CTPPSPixelDetId> > > patternVector = patternFinder_->getPatterns();

  // std::cout<<"Number of patterns = "<< patternVector.size()<<std::endl;
  // if(patternVector.size()>10){
  //  std::cout<<"!!! too many patterns, returning !!!"<<std::endl;
  //  //return;
  // }
  //loop on all the patterns
  int numberOfTracks = 0;

  for(const auto & pattern : patternVector){
    //std::cout<<"pattern found"<<std::endl;
    CTPPSPixelDetId firstHitDetId = pattern.at(0).second;
    CTPPSDetId romanPotId = firstHitDetId.getRPId();
    
    std::map<CTPPSPixelDetId, std::vector<CLHEP::Hep3Vector> > hitOnPlaneMap; //hit of the pattern organized by plane

    //loop on all the hits of the pattern
    for(const auto & hit : pattern){
      CTPPSPixelDetId hitDetId = hit.second;
      CTPPSDetId tmpRomanPotId = hitDetId.getRPId();

      if(tmpRomanPotId!=romanPotId){ //check that the hits belongs to the same tracking station
        throw cms::Exception("CTPPSPixelLocalTrackProducer") << "Hits in the pattern must belong to the same tracking station";
      }

      if(hitOnPlaneMap.find(hitDetId)==hitOnPlaneMap.end()){ //add the plane key in case it is the first hit of the plane
        std::vector<CLHEP::Hep3Vector> hitOnPlane;
        hitOnPlane.push_back(hit.first);
        hitOnPlaneMap[hitDetId] = hitOnPlane;
      }
      else hitOnPlaneMap[hitDetId].push_back(hit.first); //add the hit on a plane the key
    
    }

    trackFinder_->clear();
    trackFinder_->setHits(hitOnPlaneMap);
    trackFinder_->findTracks();
    std::vector<CTPPSPixelLocalTrack> tmpTracksVector = trackFinder_->getLocalTracks();

    if(verbosity_>2)std::cout<<"tmpTracksVector = "<<tmpTracksVector.size()<<std::endl;
    if(maxTrackPerPattern_>=0 && tmpTracksVector.size()>(uint32_t)maxTrackPerPattern_){
      if(verbosity_>2)std::cout<<" ---> To many tracks in the pattern, cleared"<<std::endl;
      continue;
    }


    for(const auto & track : tmpTracksVector){
      ++numberOfTracks;
      //std::cout<<"Track found"<<std::endl;
      edm::DetSet<CTPPSPixelLocalTrack> &tmpDetSet = foundTracks.find_or_insert(romanPotId);
        tmpDetSet.push_back(track);
    }


  }

  if(verbosity_>1) std::cout<<"Number of tracks will be saved = "<<numberOfTracks<<std::endl;

  // for(edm::DetSetVector<CTPPSPixelLocalTrack>::iterator it=foundTracks.begin(); it!=foundTracks.end(); ++it){
  for(const auto & track : foundTracks){
    if(verbosity_>1) cout<<"Track found in detId = "<<track.detId()<< " number = " << track.size() <<std::endl;
    if(maxTrackPerRomanPot_>=0 && track.size()>(uint32_t)maxTrackPerRomanPot_){
      if(verbosity_>1) std::cout<<" ---> To many tracks in the pot, cleared"<<std::endl;
      CTPPSDetId tmpRomanPotId = CTPPSDetId(track.detId());
      edm::DetSet<CTPPSPixelLocalTrack> &tmpDetSet = foundTracks[tmpRomanPotId];
      tmpDetSet.clear();
    }
  }

  iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelLocalTrack> >(foundTracks));

  return;

}

DEFINE_FWK_MODULE( CTPPSPixelLocalTrackProducer);
