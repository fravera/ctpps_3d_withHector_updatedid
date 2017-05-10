
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelClusterProducer.h"

CTPPSPixelClusterProducer::CTPPSPixelClusterProducer(const edm::ParameterSet& conf) :
  param_(conf) ,
  clusterizer_(conf){
  
  src_ = conf.getUntrackedParameter<std::string>("label");
  verbosity_ = conf.getParameter<int> ("RPixVerbosity");
	 
  tokenCTPPSPixelDigi_ = consumes<edm::DetSetVector<CTPPSPixelDigi> >(edm::InputTag(src_));
 
  produces<edm::DetSetVector<CTPPSPixelCluster> > ();
  }

CTPPSPixelClusterProducer::~CTPPSPixelClusterProducer() {

}

void CTPPSPixelClusterProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
/// get inputs
  edm::Handle<edm::DetSetVector<CTPPSPixelDigi> > rpd;
  iEvent.getByToken(tokenCTPPSPixelDigi_, rpd);
  
/// get DAQ mapping
  edm::ESHandle<CTPPSPixelDAQMapping> mapping;
  iSetup.get<CTPPSPixelDAQMappingRcd>().get("RPix",mapping);

// get analysis mask to mask channels
  edm::ESHandle<CTPPSPixelAnalysisMask> analysisMask;
  iSetup.get<CTPPSPixelAnalysisMaskRcd>().get("RPix",analysisMask);
  
// print mapping
  for (const auto &p : mapping->ROCMapping)
    LogDebug ("DAQ mapping") << "    " << p.first << " -> " << p.second;
  
// print channel mask
  for (const auto &p : analysisMask->analysisMask){
    LogDebug ("Channel Mask ") << "    " << p.first
			       << ": fullMask=" << p.second.fullMask
			       << ", number of masked pixels " << p.second.maskedPixels.size();
  }


// get calibration DB
 theGainCalibrationDB.getDB(iEvent,iSetup);

  edm::DetSetVector<CTPPSPixelCluster>  output;

// run clusterisation
  if (rpd->size())
    run(*rpd, output);

// write output
  iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelCluster> >(output));

}

void CTPPSPixelClusterProducer::run(const edm::DetSetVector<CTPPSPixelDigi> &input, edm::DetSetVector<CTPPSPixelCluster> &output){

  for (const auto &ds_digi : input)
    {
      edm::DetSet<CTPPSPixelCluster> &ds_cluster = output.find_or_insert(ds_digi.id);
 
      clusterizer_.buildClusters(ds_digi.id, ds_digi.data, ds_cluster.data, theGainCalibrationDB.getCalibs());

// --- to be removed before PR
      unsigned int cluN=0;
      for(std::vector<CTPPSPixelCluster>::iterator iit = ds_cluster.data.begin(); iit != ds_cluster.data.end(); iit++){
		if(verbosity_)	std::cout << "Cluster " << ++cluN <<" avg row " << (*iit).avg_row()<< " avg col " << (*iit).avg_col()<<" ADC.size " << (*iit).size()<<std::endl;
      }
// ---


    }
}

DEFINE_FWK_MODULE( CTPPSPixelClusterProducer);
