#ifndef SimCTPPS_RPIXRecoAnalyzer_h
#define SimCTPPS_RPIXRecoAnalyzer_h


#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h>
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigi.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSPixelDigiCollection.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelSimTopology.h"
#include "TH2D.h"

class TH1F;
class TFile;
class PSimHit;


namespace edm {
  class ParameterSet; class Event; class EventSetup;}

class CTPPSPixelRecoAnalyzer : public edm::EDAnalyzer{
  
 public:
  explicit CTPPSPixelRecoAnalyzer(const edm::ParameterSet& pset);
  virtual ~CTPPSPixelRecoAnalyzer();
  void endJob();
  void beginJob();
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  
 private:

 TH2D *hAllHits;
 TH2D *hOneHitperEvent;
 TH2D *hOneHitperEvent2;
 TH2D *hOneHitperEventCenter;
 TH2D *hOneHitperEvent2Center;
  TFile *file;
  std::string label;

  int verbosity_;
  edm::EDGetTokenT< edm::PSimHitContainer > psim_token;

  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit> > rechit_token;
  
  CTPPSPixelSimTopology theRPixDetTopology_;

};

#endif    
