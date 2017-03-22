#ifndef CTPPSPixelRawToDigi_H
#define CTPPSPixelRawToDigi_H

/** \class CTPPSPixelRawToDigi_H
 *  Plug-in module that performs Raw data to digi conversion 
 *  for pixel subdetector
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"
//#include "CondFormats/DataRecord/interface/SiPixelQualityRcd.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelDAQMappingRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
//#include "FWCore/Utilities/interface/CPUTimer.h"


//-- not needed??
/*
class SiPixelFedCablingTree;
class SiPixelFedCabling;
class SiPixelQuality;
class TH1D;
class PixelUnpackingRegions;
*/
//--


class CTPPSPixelRawToDigi : public edm::stream::EDProducer<> {
public:

  /// ctor
  explicit CTPPSPixelRawToDigi( const edm::ParameterSet& );

  /// dtor
  virtual ~CTPPSPixelRawToDigi();


//-- not needed??
//  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  /// get data, convert to digis attach againe to Event
  virtual void produce( edm::Event&, const edm::EventSetup& ) override;

private:

  edm::ParameterSet config_;

//-- not needed??
//  std::unique_ptr<SiPixelFedCablingTree> cabling_;
//  const SiPixelQuality* badPixelInfo_;
//  PixelUnpackingRegions* regions_;
//--

  edm::EDGetTokenT<FEDRawDataCollection> tFEDRawDataCollection; 

//  TH1D *hCPU, *hDigi;
//  std::unique_ptr<edm::CPUTimer> theTimer;
//  bool includeErrors;
//  bool useQuality;
//  bool debug;
//  std::vector<int> tkerrorlist;
//  std::vector<int> usererrorlist;
  std::set<unsigned int> fedIds;

/*
  edm::ESWatcher<SiPixelFedCablingMapRcd> recordWatcher;
  edm::ESWatcher<SiPixelQualityRcd> qualityWatcher;
*/
  edm::InputTag label;
  // int ndigis;
  // int nwords;
//  bool usePilotBlade;
//  bool usePhase1;
  std::string mappingLabel;
};
#endif
