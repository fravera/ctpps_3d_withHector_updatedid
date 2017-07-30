/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_RPixDetPatternFinder_H
#define RecoCTPPS_PixelLocal_RPixDetPatternFinder_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"


#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/RotationInterfaces.h"
#include "TMatrixD.h"

#include <vector>

class RPixDetPatternFinder{
  
public:
  RPixDetPatternFinder(edm::ParameterSet const& parameterSet): parameterSet_(parameterSet) {}
  
  virtual ~RPixDetPatternFinder();

  typedef struct{
    CLHEP::Hep3Vector globalPoint;
    TMatrixD          globalError;
    CTPPSPixelRecHit  recHit     ;
  } PointAndRecHit;
  typedef std::pair<PointAndRecHit, CTPPSPixelDetId> PointInPlane;
  typedef std::vector<PointInPlane> Road;
  
  void setHits(const edm::DetSetVector<CTPPSPixelRecHit> hitVector) {hitVector_ = hitVector; }
  virtual void findPattern()=0;
  void clear(){
    patternVector_.clear();
  }
  std::vector<Road> getPatterns() {return patternVector_; }
  void setGeometry(TotemRPGeometry geometry) {geometry_ = geometry; }


  
protected:
  edm::ParameterSet parameterSet_;
  edm::DetSetVector<CTPPSPixelRecHit> hitVector_;
  std::vector<Road> patternVector_;
  TotemRPGeometry geometry_;
  
};

#endif
