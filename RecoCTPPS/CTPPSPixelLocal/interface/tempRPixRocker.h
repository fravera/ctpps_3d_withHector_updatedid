/**********************************************************************
 *
 * Author: F.Ferro - INFN Genova
 * September 2016
 *
 **********************************************************************/
#ifndef RecoCTPPS_RPIX_ROCKER_2_HIT_H
#define RecoCTPPS_RPIX_ROCKER_2_HIT_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"


class tempRPixRocker{

public:

tempRPixRocker(edm::ParameterSet const& conf,  std::map<CTPPSPixelFramePosition, CTPPSPixelROCInfo> const &mapping):
  params_(conf), mapping_(mapping){}

 
  ~tempRPixRocker(){};

  void run();

private:

  const edm::ParameterSet &params_;
  const  std::map<CTPPSPixelFramePosition, CTPPSPixelROCInfo> &mapping_;
};



#endif
