#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelDAQMappingRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelAnalysisMaskRcd.h"

using namespace std;

namespace edmtest
{
  class myCTPPSPixelDAQMappingAnalyzer : public edm::EDAnalyzer
  {
  public:
    explicit  myCTPPSPixelDAQMappingAnalyzer(edm::ParameterSet const& p) {
      std::cout<<"myCTPPSPixelDAQMappingAnalyzer"<<std::endl;
          }
    explicit  myCTPPSPixelDAQMappingAnalyzer(int i) {
      std::cout<<"myCTPPSPixelDAQMappingAnalyzer "<<i<<std::endl;
    }
    virtual ~myCTPPSPixelDAQMappingAnalyzer() {
      std::cout<<"~myCTPPSPixelDAQMappingAnalyzer "<<std::endl;
    }
        virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  };

  void
  myCTPPSPixelDAQMappingAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){

    std::cout<<"###myCTPPSPixelDAQMappingAnalyzer::analyze"<<std::endl;
    std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
    std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;

    /*edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelDAQMappingRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"CTPPSPixelDAQMappingRcd"<<"\" does not exist "<<std::endl;
    }*/

    //this part gets the handle of the event source and the record (i.e. the Database)
    edm::ESHandle<CTPPSPixelDAQMapping> mapping;
    context.get<CTPPSPixelDAQMappingRcd>().get(mapping);

    // print mapping
    printf("* DAQ mapping\n");
    for (const auto &p : mapping->ROCMapping)
    std::cout << "    " << p.first << " -> " << p.second << std::endl;

    /*edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelAnalysisMaskRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"CTPPSPixelAnalysisMaskRcd"<<"\" does not exist "<<std::endl;
    }*/

    // get analysis mask to mask channels
    edm::ESHandle<CTPPSPixelAnalysisMask> analysisMask;
    context.get<CTPPSPixelAnalysisMaskRcd>().get(analysisMask);

    // print mask
    printf("* mask\n");
    for (const auto &p : analysisMask->analysisMask)
      cout << "    " << p.first
        << ": fullMask=" << p.second.fullMask
        << ", number of masked channels " << p.second.maskedPixels.size() << endl;


  }
  DEFINE_FWK_MODULE(myCTPPSPixelDAQMappingAnalyzer);
}
