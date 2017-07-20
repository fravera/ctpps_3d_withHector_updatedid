
#include "RecoCTPPS/PixelLocal/interface/CTPPSPixelRecHitProducer.h"

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

CTPPSPixelRecHitProducer::CTPPSPixelRecHitProducer(const edm::ParameterSet& conf) :
  param_(conf), cluster2hit_(conf)
{
  
  src_ = conf.getParameter<std::string>("label");
  verbosity_ = conf.getUntrackedParameter<int> ("RPixVerbosity");
	 
  tokenCTPPSPixelCluster_ = consumes<edm::DetSetVector<CTPPSPixelCluster> >(edm::InputTag(src_));
 
  produces<edm::DetSetVector<CTPPSPixelRecHit> > ();
}

CTPPSPixelRecHitProducer::~CTPPSPixelRecHitProducer() {

}

void CTPPSPixelRecHitProducer::fillDescriptions(edm::ConfigurationDescriptions & descriptions){
  edm::ParameterSetDescription desc;
  desc.addUntracked<int>("RPixVerbosity",0);
  desc.add<std::string>("label", "ctppsPixelClusters");
  descriptions.add("ctppsPixelRecHits", desc);
}

// ------------ method called to produce the data  ------------
void CTPPSPixelRecHitProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   
  if(verbosity_)std::cout << "  ****************************************************** " << std::endl;
  if(verbosity_)std::cout << "  ****        Inside CTPPSPixelRecHitProducer        ***** " << std::endl;
  if(verbosity_)std::cout << "  ****************************************************** " << std::endl;

// Step A: get inputs

	edm::Handle<edm::DetSetVector<CTPPSPixelCluster> > rpCl;
	iEvent.getByToken(tokenCTPPSPixelCluster_, rpCl);

 // get geometry
 //----------------------------------

	edm::ESHandle<CTPPSGeometry> geometry;
//	iSetup.get<VeryForwardMisalignedGeometryRecord>().get(geometry);
	iSetup.get<VeryForwardRealGeometryRecord>().get(geometry);

	geometryWatcher.check(iSetup);

// dry checks to be removed
/*	CTPPSPixelDetId iid(0,2,3,1);
	std::cout << iid << std::endl;
	std::cout << iid.rawId() << std::endl;
	unsigned int rpId = 2023292928;

	CLHEP::Hep3Vector localV(-4.43825,2.05224,0.115);
	CLHEP::Hep3Vector globalV = geometry->localToGlobal(rpId,localV);


	std::cout << "id: "<< rpId <<"   local " << localV <<"   to global "<<globalV<< std::endl;
*/
//---------------------------------------------



	edm::DetSetVector<CTPPSPixelRecHit>  output;

// run reconstruction
	if (rpCl->size())
	  run(*rpCl, output);

        // Step D: write output to file
//==============
	unsigned int idSS = 0;
	for(const auto & ds_rh : output){
	  idSS++;
	  if(verbosity_)std::cout << "ID :  " << ds_rh.id << std::endl;
	}
	if(idSS >=5 ){

	  for(const auto & ds_rh2 : output){
	    for (const auto & _rh : ds_rh2.data){

	      CLHEP::Hep3Vector localV(_rh.getPoint().x(),_rh.getPoint().y(),_rh.getPoint().z() );
	      CLHEP::Hep3Vector globalV = geometry->localToGlobal(ds_rh2.id,localV);
	      if(verbosity_)std::cout << "ID : " << ds_rh2.id << " hit  " << _rh.getPoint().x()<<" "<<_rh.getPoint().y()<<" " <<_rh.getPoint().z()<< "   "<< globalV.x() << " " << globalV.y() <<" " <<std::setprecision(6) << globalV.z() <<std::endl;

	    }
	  }
	}
	
//==============       
	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelRecHit> >(output));

}

void CTPPSPixelRecHitProducer::run(const edm::DetSetVector<CTPPSPixelCluster> &input, edm::DetSetVector<CTPPSPixelRecHit> &output){

  for (const auto &ds_cluster : input)
    {
      edm::DetSet<CTPPSPixelRecHit> &ds_rechit = output.find_or_insert(ds_cluster.id);


// call here an RPix CPE to calculate the cluster parameters and, maybe, to convert it into a rechit 
         cluster2hit_.buildHits(ds_cluster.id, ds_cluster.data, ds_rechit.data);



	
//-----------------------------------
      unsigned int rechitN=0;
      for(std::vector<CTPPSPixelRecHit>::iterator iit = ds_rechit.data.begin(); iit != ds_rechit.data.end(); iit++){
	++rechitN;
	if(verbosity_)	std::cout << "Rechit " << rechitN <<" x " << (*iit).getPoint().x()<< " y " << (*iit).getPoint().y()<< " z " <<  (*iit).getPoint().z() << std::endl;





      }

      if(verbosity_)	std::cout << "Rechit Number = " << rechitN << std::endl; 




//-----------------------------------
    }
}

DEFINE_FWK_MODULE( CTPPSPixelRecHitProducer);
