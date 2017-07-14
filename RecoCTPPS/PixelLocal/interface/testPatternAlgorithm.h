/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_testPatternAlgorithm
#define RecoCTPPS_PixelLocal_testPatternAlgorithm

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "RecoCTPPS/PixelLocal/interface/RPixDetPatternFinder.h"


#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "CLHEP/Vector/ThreeVector.h"

#include <vector>

class testPatternAlgorithm : public RPixDetPatternFinder{

	public:
		testPatternAlgorithm(edm::ParameterSet const& parameterSet) : RPixDetPatternFinder(parameterSet) {}
		~testPatternAlgorithm() {}

		void findPattern(){
			return;
		}

		
};

#endif