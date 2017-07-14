/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_testTrackingAlgorithm
#define RecoCTPPS_PixelLocal_testTrackingAlgorithm

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"
#include "RecoCTPPS/PixelLocal/interface/RPixDetTrackFinder.h"


#include <vector>
#include <map>
#include "TMatrixD.h"
#include "TVectorD.h"


class testTrackingAlgorithm : public RPixDetTrackFinder{

	public:
		testTrackingAlgorithm(edm::ParameterSet const& parameterSet) : RPixDetTrackFinder(parameterSet) {}
		~testTrackingAlgorithm() {}
		
		void findTracks(){
			std::map<CTPPSPixelDetId, std::vector<CLHEP::Hep3Vector> >::iterator mapIt = hitMap_.begin();
			CTPPSPixelDetId tmpDetId = mapIt->first;
			tmpDetId.setPlane(0);
			CTPPSPixelDetId firstPlane = tmpDetId;
			tmpDetId.setPlane(5);
			CTPPSPixelDetId lastPlane  = tmpDetId;

			std::vector<CLHEP::Hep3Vector> firstPlaneHits = hitMap_[firstPlane];
			std::vector<CLHEP::Hep3Vector> lastPlaneHits  = hitMap_[lastPlane] ;

			for(const auto & hitFirstPlane : firstPlaneHits){
				double x0 = hitFirstPlane.x();
				double y0 = hitFirstPlane.y();
				double z0 = hitFirstPlane.z();
				for(const auto & hitLastPlane : lastPlaneHits){
					double x5 = hitLastPlane.x();
					double y5 = hitLastPlane.y();
					double z5 = hitLastPlane.z();

					double tx = (x5-x0)/(z5-z0);
					double ty = (y5-y0)/(z5-z0);
					
					double tmpArray[4] = {x0,y0,tx,ty};
					TVectorD tmpVector(4,tmpArray);

					double tmpMatrixArray[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
					TMatrixD tmpMatrix(4,4,tmpMatrixArray);

					CTPPSPixelLocalTrack track(z0, tmpVector, tmpMatrix, 1.);

					localTrackVector_.push_back(track);

				}
			}
		};

};

#endif