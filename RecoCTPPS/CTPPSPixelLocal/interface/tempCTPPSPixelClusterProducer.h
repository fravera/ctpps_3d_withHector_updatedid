/**********************************************************************
 *
 * Author: F.Ferro - INFN Genova
 * September 2016
 *
 **********************************************************************/
#ifndef RecoCTPPS_CTPPSPixelLocal_CTPPSPixelClusterProducer
#define RecoCTPPS_CTPPSPixelLocal_CTPPSPixelClusterProducer

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
//#include "DataFormats/Common/interface/DetSetVectorNew.h"
//#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "RecoCTPPS/CTPPSPixelLocal/interface/RPixDetClusterizer.h" 
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include "FWCore/Framework/interface/ESWatcher.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "Geometry/VeryForwardRPTopology/interface/RPTopology.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "CondFormats/DataRecord/interface/CTPPSPixelReadoutRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"

#include "RecoCTPPS/CTPPSPixelLocal/interface/tempRPixRocker.h"

#include <vector>
#include <set>




class tempCTPPSPixelClusterProducer : public edm::stream::EDProducer<>
{
public:
  explicit tempCTPPSPixelClusterProducer(const edm::ParameterSet& param);
 
  ~tempCTPPSPixelClusterProducer();

 virtual void produce(edm::Event&, const edm::EventSetup&) override;

private:
 edm::ParameterSet param_;
 int verbosity_;
 
 edm::InputTag src_;
 edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelDigi>> tokenCTPPSPixelDigi_;
  
// edm::ESWatcher<VeryForwardRealGeometryRecord> geometryWatcher;

 RPixDetClusterizer clusterizer_;
  
 void run(const edm::DetSetVector<CTPPSPixelDigi> &input, edm::DetSetVector<CTPPSPixelCluster> &output);
 
};



#endif
