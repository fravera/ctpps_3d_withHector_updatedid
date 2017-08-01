#include "RecoCTPPS/PixelLocal/interface/testTrackingAlgorithm.h"


#include "TMatrixD.h"
#include "TVectorD.h"

//------------------------------------------------------------------------------------------------//

testTrackingAlgorithm::testTrackingAlgorithm(edm::ParameterSet const& parameterSet) : RPixDetTrackFinder(parameterSet){

}

//------------------------------------------------------------------------------------------------//
		
testTrackingAlgorithm::~testTrackingAlgorithm() {

}


//------------------------------------------------------------------------------------------------//

		
void testTrackingAlgorithm::findTracks(){
	std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> >::iterator mapIt = hitMap_.begin();
	CTPPSPixelDetId tmpDetId = mapIt->first;
	tmpDetId.setPlane(0);
	CTPPSPixelDetId firstPlane = tmpDetId;
	tmpDetId.setPlane(5);
	CTPPSPixelDetId lastPlane  = tmpDetId;

	std::vector<RPixDetPatternFinder::PointInPlane> firstPlaneHits = hitMap_[firstPlane];
	std::vector<RPixDetPatternFinder::PointInPlane> lastPlaneHits  = hitMap_[lastPlane ];

	for(const auto & hitFirstPlane : firstPlaneHits){
		double x0 = hitFirstPlane.globalPoint.x();
		double y0 = hitFirstPlane.globalPoint.y();
		double z0 = hitFirstPlane.globalPoint.z();
		for(const auto & hitLastPlane : lastPlaneHits){
			double x5 = hitLastPlane.globalPoint.x();
			double y5 = hitLastPlane.globalPoint.y();
			double z5 = hitLastPlane.globalPoint.z();

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

	return;
}
