/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_testTrackingAlgorithm_H
#define RecoCTPPS_PixelLocal_testTrackingAlgorithm_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"
#include "RecoCTPPS/PixelLocal/interface/RPixDetTrackFinder.h"


#include <vector>
#include <map>


class testTrackingAlgorithm : public RPixDetTrackFinder{

	public:
		testTrackingAlgorithm(edm::ParameterSet const& parameterSet);
		~testTrackingAlgorithm();
		
		void findTracks() override;
		void initialize() override {return;}
};

#endif