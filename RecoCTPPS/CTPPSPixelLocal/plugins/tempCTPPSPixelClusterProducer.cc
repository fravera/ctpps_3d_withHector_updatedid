
#include "RecoCTPPS/CTPPSPixelLocal/interface/tempCTPPSPixelClusterProducer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//needed for the geometry:
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include <vector>
#include <memory>
#include <string>
#include <iostream>
//#include <cstdlib> // I need it for random numbers

tempCTPPSPixelClusterProducer::tempCTPPSPixelClusterProducer(const edm::ParameterSet& conf) :
  param_(conf) ,
  clusterizer_(conf){
  
  src_ = conf.getUntrackedParameter<std::string>("label");
  verbosity_ = conf.getParameter<int> ("RPixVerbosity");
	 
  tokenCTPPSPixelDigi_ = consumes<edm::DetSetVector<CTPPSPixelDigi> >(edm::InputTag(src_));
 
  produces<edm::DetSetVector<CTPPSPixelCluster> > ();
}

tempCTPPSPixelClusterProducer::~tempCTPPSPixelClusterProducer() {

}


// ------------ method called to produce the data  ------------
void tempCTPPSPixelClusterProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   
 
// Step A: get inputs

	edm::Handle<edm::DetSetVector<CTPPSPixelDigi> > rpd;
	iEvent.getByToken(tokenCTPPSPixelDigi_, rpd);

 // get mappings
 //----------------------------------
/*
	edm::ESHandle<TotemRPGeometry> geometry;
	iSetup.get<VeryForwardMeasuredGeometryRecord>().get(geometry);

	geometryWatcher.check(iSetup);
*/
  // get DAQ mapping
	edm::ESHandle<CTPPSPixelDAQMapping> mapping;
	iSetup.get<CTPPSPixelReadoutRcd>().get("RPix", mapping);

  // get analysis mask to mask channels
	edm::ESHandle<CTPPSPixelAnalysisMask> analysisMask;
	iSetup.get<CTPPSPixelReadoutRcd>().get("RPix", analysisMask);

// dry checks to be removed
/*
 // print mapping
  printf("* DAQ mapping\n");
  for (const auto &p : mapping->ROCMapping)
    std::cout << "    " << p.first << " -> " << p.second << std::endl;

  // print mapping
  printf("* mask\n");
  for (const auto &p : analysisMask->analysisMask){
    std::cout << "    " << p.first
      << ": fullMask=" << p.second.fullMask
	      << ", number of masked pixels " << p.second.maskedPixels.size() << std::endl;
  }

*/


  
//---------------------------------------------

	tempRPixRocker tRPR(param_,mapping->ROCMapping);

	tRPR.run();

  for (const auto &pp : mapping->fedIds() ){
    std::cout << "           " << pp << std::endl;
  }
  std::set<unsigned int> fedIds = mapping->fedIds() ;
  for (auto aFed = fedIds.begin(); aFed != fedIds.end(); ++aFed) {
    int fedId = *aFed;
    std::cout << "                            " << fedId << std::endl;
  }


	
      //     theClusterVector.reserve(400);
      //    theClusterVector.clear();

	edm::DetSetVector<CTPPSPixelCluster>  output;

// run clusterisation
	if (rpd->size())
	  run(*rpd, output);


        // Step D: write output to file
       
	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelCluster> >(output));

}

void tempCTPPSPixelClusterProducer::run(const edm::DetSetVector<CTPPSPixelDigi> &input, edm::DetSetVector<CTPPSPixelCluster> &output){

  for (const auto &ds_digi : input)
    {
      edm::DetSet<CTPPSPixelCluster> &ds_cluster = output.find_or_insert(ds_digi.id);
 
      clusterizer_.buildClusters(ds_digi.id, ds_digi.data, ds_cluster.data);

//-----------------------------------
      unsigned int cluN=0;
      for(std::vector<CTPPSPixelCluster>::iterator iit = ds_cluster.data.begin(); iit != ds_cluster.data.end(); iit++){
	
	if(verbosity_)	std::cout << "Cluster " << ++cluN <<" avg row " << (*iit).avg_row()<< " avg col " << (*iit).avg_col()<<std::endl;





      }

//-----------------------------------
    }
}

DEFINE_FWK_MODULE( tempCTPPSPixelClusterProducer);
