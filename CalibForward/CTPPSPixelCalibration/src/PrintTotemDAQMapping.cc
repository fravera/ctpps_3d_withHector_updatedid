/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors: 
*  Jan Kašpar (jan.kaspar@gmail.com) 
*
****************************************************************************/

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "CondFormats/DataRecord/interface/CTPPSPixelDAQMappingRcd.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelAnalysisMaskRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"

//----------------------------------------------------------------------------------------------------

/**
 *\brief Prints the DAQ mapping loaded by TotemDAQMappingESSourceXML.
 **/
class PrintTotemDAQMapping : public edm::one::EDAnalyzer<>
{
  public:
    PrintTotemDAQMapping(const edm::ParameterSet &ps) {}
    ~PrintTotemDAQMapping() {}

  private:
    virtual void analyze(const edm::Event &e, const edm::EventSetup &es) override;
};

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

void PrintTotemDAQMapping::analyze(const edm::Event&, edm::EventSetup const& es)
{
  // get mapping
  ////ESHandle<TotemDAQMapping> mapping;
  ////es.get<TotemReadoutRcd>().get(mapping);

  // get DAQ mapping
  edm::ESHandle<CTPPSPixelDAQMapping> mapping;
  es.get<CTPPSPixelDAQMappingRcd>().get("RPix", mapping);
    
  // get analysis mask to mask channels
  ESHandle<CTPPSPixelAnalysisMask> analysisMask;
  es.get<CTPPSPixelDAQMappingRcd>().get("RPix", analysisMask);

  // print mapping
  printf("* DAQ mapping\n");
  for (const auto &p : mapping->ROCMapping)
    cout << "    " << p.first << " -> " << p.second << endl;

  
  // print mapping
  printf("* mask\n");
  for (const auto &p : analysisMask->analysisMask)
    cout << "    " << p.first
      << ": fullMask=" << p.second.fullMask
      << ", number of masked channels " << p.second.maskedPixels.size() << endl;

  // Write DAQ Mapping to sqlite file:
  const CTPPSPixelDAQMapping* pCTPPSPixelDAQMapping = mapping.product();
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if( poolDbService.isAvailable() )
    poolDbService->writeOne( pCTPPSPixelDAQMapping, poolDbService->currentTime(), /*m_record*/ "CTPPSPixelDAQMappingRcd"  );

  
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PrintTotemDAQMapping);
