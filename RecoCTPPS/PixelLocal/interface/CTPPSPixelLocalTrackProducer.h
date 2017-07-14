/*
 *
* This is a part of CTPPS offline software.
* Author:
*   Fabrizio Ferro (ferro@ge.infn.it)
*   Enrico Robutti (robutti@ge.infn.it)
*   Fabio Ravera   (fabio.ravera@cern.ch)
*
*/
#ifndef RecoCTPPS_PixelLocal_CTPPSPixelLocalTrackProducer
#define RecoCTPPS_PixelLocal_CTPPSPixelLocalTrackProducer

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
 
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "RecoCTPPS/PixelLocal/interface/RPixDetPatternFinder.h"
#include "RecoCTPPS/PixelLocal/interface/RPixDetTrackFinder.h"
#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"


#include <string>
#include <vector>

class CTPPSPixelLocalTrackProducer : public edm::stream::EDProducer<>
{
public:
  explicit CTPPSPixelLocalTrackProducer(const edm::ParameterSet& parameterSet);
 
  ~CTPPSPixelLocalTrackProducer();

  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);

private:
  edm::ParameterSet parameterSet_;
  int verbosity_;
 
  edm::InputTag inputTag_;
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit>> tokenCTPPSPixelRecHit_;
  edm::ESWatcher<VeryForwardMisalignedGeometryRecord> geometryWatcher_;

  RPixDetPatternFinder *patternFinder_;
  RPixDetTrackFinder   *trackFinder_;
  
  void run(const edm::DetSetVector<CTPPSPixelRecHit> &input, edm::DetSetVector<CTPPSPixelLocalTrack> &output);

};



#endif
