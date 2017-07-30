/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_RPixDetTrackFinder_H
#define RecoCTPPS_PixelLocal_RPixDetTrackFinder_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"
#include "RecoCTPPS/PixelLocal/interface/RPixDetPatternFinder.h"


#include "CLHEP/Vector/ThreeVector.h"

#include <vector>
#include <map>

class RPixDetTrackFinder{

	public:
		RPixDetTrackFinder(edm::ParameterSet const& parameterSet): parameterSet_(parameterSet), romanPotId_(CTPPSPixelDetId(0, 2, 3, 0)) {}
		//romanPotId_ is needed to be defined in order to 

		virtual ~RPixDetTrackFinder();

		void setHits(const std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> > hitMap) {hitMap_ = hitMap; }
  		virtual void findTracks()=0;
		void clear(){
			hitMap_.clear();
			localTrackVector_.clear();
		}
		std::vector<CTPPSPixelLocalTrack> getLocalTracks() {return localTrackVector_; }
  		void setRomanPotId(CTPPSPixelDetId rpId) {romanPotId_ = rpId;};


	protected:
		edm::ParameterSet parameterSet_;
		std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> > hitMap_;
		std::vector<CTPPSPixelLocalTrack>  localTrackVector_;
		CTPPSPixelDetId  romanPotId_;

};

#endif
