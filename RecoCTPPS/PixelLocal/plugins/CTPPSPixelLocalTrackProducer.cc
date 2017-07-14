
#include "RecoCTPPS/PixelLocal/interface/CTPPSPixelLocalTrackProducer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include "RecoCTPPS/PixelLocal/interface/testPatternAlgorithm.h"
#include "RecoCTPPS/PixelLocal/interface/testTrackingAlgorithm.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"


//------------------------------------------------------------------------------------------------//

CTPPSPixelLocalTrackProducer::CTPPSPixelLocalTrackProducer(const edm::ParameterSet& parameterSet) :
	parameterSet_(parameterSet){

  inputTag_  = parameterSet_.getParameter<std::string>("label");
  verbosity_ = parameterSet_.getUntrackedParameter<int> ("RPixVerbosity");

	std::string patterFinderAlgorithm = parameterSet_.getParameter<std::string>("patterFinderAlgorithm");
	std::string trackFitterAlgorithm  = parameterSet_.getParameter<std::string>("trackFinderAlgorithm" );

	// patter algorithm selector
	if(patterFinderAlgorithm == "testPatternAlgorithm"){
		patternFinder_ = new testPatternAlgorithm(parameterSet_);
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
  desc.add<std::string>("label", "ctppsPixelRecHits");
  desc.add<std::string>("patterFinderAlgorithm", "testPatternAlgorithm" );
  desc.add<std::string>("trackFinderAlgorithm" , "testTrackingAlgorithm");
  descriptions.add("ctppsPixelTracks", desc);
}

//------------------------------------------------------------------------------------------------//

void CTPPSPixelLocalTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
	// Step A: get inputs
	edm::Handle<edm::DetSetVector<CTPPSPixelRecHit> > recHits;
	iEvent.getByToken(tokenCTPPSPixelRecHit_, recHits);
	const edm::DetSetVector<CTPPSPixelRecHit> &recHitVector = *recHits;

	// get geometry
	edm::ESHandle<TotemRPGeometry> geometryHandler;
	//	iSetup.get<VeryForwardMisalignedGeometryRecord>().get(geometry);
	iSetup.get<VeryForwardRealGeometryRecord>().get(geometryHandler);
	const TotemRPGeometry &geometry = *geometryHandler;

	geometryWatcher_.check(iSetup);

	edm::DetSetVector<CTPPSPixelLocalTrack> foundTracks;

	// Pattern finder

  patternFinder_->clear();
	patternFinder_->setHits(recHitVector);
	patternFinder_->setGeometry(geometry);
	patternFinder_->findPattern();
	std::vector<std::vector<std::pair<CLHEP::Hep3Vector, CTPPSPixelDetId> > > patternVector = patternFinder_->getPatterns();

	//loop on all the patterns
	for(const auto & pattern : patternVector){
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

		for(const auto & track : tmpTracksVector){
			edm::DetSet<CTPPSPixelLocalTrack> &tmpDetSet = foundTracks.find_or_insert(romanPotId);
    	tmpDetSet.push_back(track);
		}
	}

  iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelLocalTrack> >(foundTracks));

}

DEFINE_FWK_MODULE( CTPPSPixelLocalTrackProducer);
