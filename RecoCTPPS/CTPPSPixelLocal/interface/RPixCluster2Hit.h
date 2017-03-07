/**********************************************************************
 *
 * Author: F.Ferro - INFN Genova
 * September 2016
 *
 **********************************************************************/
#ifndef RecoCTPPS_RPIX_CLUSTER_2_HIT_H
#define RecoCTPPS_RPIX_CLUSTER_2_HIT_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelSimTopology.h"

#include <vector>
#include <set>



class RPixCluster2Hit{

public:

  RPixCluster2Hit(edm::ParameterSet const& conf);

  void buildHits(unsigned int detId, const std::vector<CTPPSPixelCluster> &clusters, std::vector<CTPPSPixelRecHit> &hits);
  void make_hit(CTPPSPixelCluster aCluster,  std::vector<CTPPSPixelRecHit> &hits );
  ~RPixCluster2Hit();


private:

  const edm::ParameterSet &params_;
  int verbosity_;
  const CTPPSPixelSimTopology *theTopology;

};



#endif
