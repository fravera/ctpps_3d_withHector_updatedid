/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_RPixDetTrackFinder
#define RecoCTPPS_PixelLocal_RPixDetTrackFinder

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

#include <vector>
#include <map>


class RPixDetTrackFinder{

	public:
		RPixDetTrackFinder(edm::ParameterSet const& parameterSet) : 
			parameterSet_(parameterSet) {};
		virtual ~RPixDetTrackFinder(){};

		void setHits(const std::map<CTPPSPixelDetId, std::vector<CLHEP::Hep3Vector> > hitMap) {hitMap_ = hitMap; };
		virtual void findTracks();
		void clear(){
			hitMap_.clear();
			localTrackVector_.clear();
		};
		std::vector<CTPPSPixelLocalTrack> getLocalTracks() {return localTrackVector_; };

	protected:
		edm::ParameterSet parameterSet_;
		std::map<CTPPSPixelDetId, std::vector<CLHEP::Hep3Vector> > hitMap_;
		std::vector<CTPPSPixelLocalTrack>  localTrackVector_;

};

#endif