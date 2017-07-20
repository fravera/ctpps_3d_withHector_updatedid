/**********************************************************************
 *
 * Author: F.Ferro - INFN Genova
 * December 2016
 *
 **********************************************************************/
#ifndef RecoCTPPS_PixelLocal_patternProducer
#define RecoCTPPS_PixelLocal_patternProducer

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
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "RecoCTPPS/PixelLocal/interface/RPixClusterToHit.h" 

#include "FWCore/Framework/interface/ESWatcher.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "Geometry/VeryForwardRPTopology/interface/RPTopology.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"


#include <vector>
#include <set>


#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"


class patternProducer : public edm::stream::EDProducer<>
{
public:
  explicit patternProducer(const edm::ParameterSet& param);
 
  ~patternProducer();

  typedef std::pair<CLHEP::Hep3Vector,CTPPSPixelDetId> PointInPlane;
  typedef std::vector<PointInPlane> Road;
  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
  virtual void produce(edm::Event&, const edm::EventSetup&) override;

  virtual void beginStream(const edm::StreamID) override;
  virtual void endStream() override;
  
private:
  edm::ParameterSet param_;
  int verbosity_;
  double roadRadius_;
  unsigned int minRoadSize_;
  unsigned int maxRoadSize_;

  edm::InputTag src_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> tokenCTPPSPixelRecHit_;
  
  edm::ESWatcher<VeryForwardMisalignedGeometryRecord> geometryWatcher;
  void run(const edm::DetSetVector<CTPPSPixelRecHit> &input, const TotemRPGeometry & geometry, std::vector<Road> &roads);
  
// void run(const edm::DetSetVector<CTPPSPixelCluster> &input, edm::DetSetVector<CTPPSPixelRecHit> &output);
 
   TFile* fFile;
   TH2F * plane_plus_0;
   TH2F * plane_plus_1;
   TH2F * plane_plus_2;
   TH2F * plane_plus_3;
   TH2F * plane_plus_4;
   TH2F * plane_plus_5;

   TH2F * plane_minus_0;
   TH2F * plane_minus_1;
   TH2F * plane_minus_2;
   TH2F * plane_minus_3;
   TH2F * plane_minus_4;
   TH2F * plane_minus_5;

};



#endif
