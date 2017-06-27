#include <iostream>


#include "RecoCTPPS/CTPPSPixelLocal/interface/RPixCluster2Hit.h"



RPixCluster2Hit::RPixCluster2Hit(edm::ParameterSet const& conf):
  params_(conf), theTopology(new CTPPSPixelSimTopology(conf))
{
verbosity_ = conf.getParameter<int>("RPixVerbosity");

//SeedADCThreshold_ = conf.getParameter<int>("SeedADCThreshold");
//ADCThreshold_ = conf.getParameter<int>("ADCThreshold");
//ElectronADCGain_ = conf.getParameter<double>("ElectronADCGain");
}

RPixCluster2Hit::~RPixCluster2Hit(){
  delete theTopology;
}

void RPixCluster2Hit::buildHits(unsigned int detId, const std::vector<CTPPSPixelCluster> &clusters, std::vector<CTPPSPixelRecHit> &hits)
{
  
  if(verbosity_) std::cout<<" RPixCluster2Hit "<<detId<<" received cluster array of size ="<<clusters.size()<<std::endl;
 
  for(unsigned int i=0; i<clusters.size();i++){
     if(verbosity_)  std::cout<<"         received cluster avg row,col "<< clusters[i].avg_row()<<","<< clusters[i].avg_col()<< std::endl;

     make_hit(clusters[i],hits);


  }





}



void RPixCluster2Hit::make_hit(CTPPSPixelCluster aCluster,  std::vector<CTPPSPixelRecHit> &hits ){

// take a cluster, generate a rec hit and push it in the rec hit vector

// get information from the cluster 

// get the whole cluster size and row/col size
  int thisClusterSize = aCluster.size();
  int thisClusterRowSize = aCluster.sizeRow();
  int thisClusterColSize = aCluster.sizeCol();

// get the minimum pixel row/col
  int thisClusterMinRow = aCluster.minPixelRow();
  int thisClusterMinCol = aCluster.minPixelCol();

  CTPPSPixelIndices pxlInd;

  if(thisClusterSize<=0) throw cms::Exception("RPixCluster2Hit") << "Fatal: CTPPS cluster size <= 0!" << std::endl;

// calculate "on edge" flag
  bool anEdgePixel = false;
  if(aCluster.minPixelRow() == 0 || aCluster.minPixelCol() == 0 ||  (aCluster.minPixelRow()+aCluster.rowSpan()) == (pxlInd.getDefaultRowDetSize()-1) ||  (aCluster.minPixelCol()+aCluster.colSpan()) == (pxlInd.getDefaultColDetSize()-1) ) anEdgePixel = true;

// check for bad (ADC=0) pixels in cluster  // CHECK IF IT IS TRUE IN CLUSTERIZER !!!!!!!!!!!!!!!!!!!!! the dead/noisy pixels are set with adc=0 in the calibration. 
  bool aBadPixel = false;
  for(int i = 0; i < thisClusterSize; i++){
    if(aCluster.pixelADC(i)==0) aBadPixel = true;
  } 

// check for spanning two ROCs

  bool twoRocs = false;
  int currROCId = pxlInd.getROCId(aCluster.pixelCol(0),aCluster.pixelRow(0) ); 
  if(thisClusterSize>1)
  for(int i = 1; i < thisClusterSize; i++){
    if(  pxlInd.getROCId(aCluster.pixelCol(i),aCluster.pixelRow(i) ) != currROCId ){
      twoRocs = true;
      break;
    }
  }

// tentative +++++

  double avgLocalX = 0;
  double avgLocalY = 0;
  double avgWLocalX = 0;
  double avgWLocalY = 0;
  double Weights = 0;

  double minPxlX = 0;
  double minPxlY = 0;
  double maxPxlX = 0;
  double maxPxlY = 0;
  double avgPxlX = 0;
  double avgPxlY = 0;
  if(verbosity_)std::cout << " INSIDE RPIXCluster2Hit::make_hit " << std::endl;
  if(verbosity_)std::cout << " hit pixels: " << std::endl;
  for(int i = 0; i < thisClusterSize; i++){

    if(verbosity_)std::cout <<aCluster.pixelRow(i)<< " "<< aCluster.pixelCol(i)<<" " << aCluster.pixelADC(i)<<" " << std::endl;
    theTopology->PixelRange(aCluster.pixelRow(i),aCluster.pixelCol(i),minPxlX,maxPxlX,minPxlY, maxPxlY);
    avgPxlX = minPxlX+(maxPxlX-minPxlX)/2.;
    avgPxlY = minPxlY+(maxPxlY-minPxlY)/2.;
    
    avgWLocalX += avgPxlX*aCluster.pixelADC(i);
    avgWLocalY += avgPxlY*aCluster.pixelADC(i);
    Weights += aCluster.pixelADC(i);

  } 

  avgLocalX = avgWLocalX / Weights;
  avgLocalY = avgWLocalY / Weights;
  
//temporary +++++
  LocalPoint lp(avgLocalX,avgLocalY,0);
  LocalError le(0,0,0);
  CTPPSPixelRecHit rh(lp,le,anEdgePixel,aBadPixel,twoRocs,thisClusterMinRow,thisClusterMinCol,thisClusterSize,thisClusterRowSize,thisClusterColSize);
  if(verbosity_)std::cout << lp << std::endl;

  hits.push_back(rh);
//+++++++++

  return;

}





