#include <iostream>


#include "RecoCTPPS/PixelLocal/interface/RPixClusterToHit.h"



RPixClusterToHit::RPixClusterToHit(edm::ParameterSet const& conf):
  params_(conf), theTopology(new CTPPSPixelSimTopology(conf))
{
verbosity_ = conf.getUntrackedParameter<int>("RPixVerbosity");

}

RPixClusterToHit::~RPixClusterToHit(){
  delete theTopology;
}

void RPixClusterToHit::buildHits(unsigned int detId, const std::vector<CTPPSPixelCluster> &clusters, std::vector<CTPPSPixelRecHit> &hits)
{
  
  if(verbosity_) std::cout<<" RPixClusterToHit "<<detId<<" received cluster array of size ="<<clusters.size()<<std::endl;
 
  for(unsigned int i=0; i<clusters.size();i++){
     if(verbosity_)  std::cout<<"         received cluster avg row,col "<< clusters[i].avg_row()<<","<< clusters[i].avg_col()<< std::endl;

     make_hit(clusters[i],hits);


  }





}



void RPixClusterToHit::make_hit(CTPPSPixelCluster aCluster,  std::vector<CTPPSPixelRecHit> &hits ){

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

  if(thisClusterSize<=0) throw cms::Exception("RPixClusterToHit") << "Fatal: CTPPS cluster size <= 0!" << std::endl;

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


  double avgWLocalX = 0;
  double avgWLocalY = 0;
  double weights = 0;
  double weightedVarianceX = 0.;
  double weightedVarianceY = 0.;

  if(verbosity_)
    std::cout << " INSIDE RPIXClusterToHit::make_hit " << std::endl;
  if(verbosity_)
    std::cout << " hit pixels: " << std::endl;

  for(int i = 0; i < thisClusterSize; i++){

    if(verbosity_)std::cout <<aCluster.pixelRow(i)<< " "<< aCluster.pixelCol(i)<<" " << aCluster.pixelADC(i)<<" " << std::endl;

    double minPxlX = 0;
    double minPxlY = 0;
    double maxPxlX = 0;
    double maxPxlY = 0;
    theTopology->PixelRange(aCluster.pixelRow(i),aCluster.pixelCol(i),minPxlX,maxPxlX,minPxlY, maxPxlY);
    double halfSizeX = (maxPxlX-minPxlX)/2.;
    double halfSizeY = (maxPxlY-minPxlY)/2.;
    double avgPxlX = minPxlX + halfSizeX;
    double avgPxlY = minPxlY + halfSizeY;
//error propagation
    weightedVarianceX += aCluster.pixelADC(i)*aCluster.pixelADC(i)*halfSizeX*halfSizeX/3.;
    weightedVarianceY += aCluster.pixelADC(i)*aCluster.pixelADC(i)*halfSizeY*halfSizeY/3.;
    
    avgWLocalX += avgPxlX*aCluster.pixelADC(i);
    avgWLocalY += avgPxlY*aCluster.pixelADC(i);
    weights += aCluster.pixelADC(i);

  } 

  double avgLocalX = avgWLocalX / weights;
  double avgLocalY = avgWLocalY / weights;
  
  double varianceX = weightedVarianceX/weights/weights;
  double varianceY = weightedVarianceY/weights/weights;

//temporary +++++
  LocalPoint lp(avgLocalX,avgLocalY,0);
  LocalError le(varianceX,0,varianceY);
  CTPPSPixelRecHit rh(lp,le,anEdgePixel,aBadPixel,twoRocs,thisClusterMinRow,thisClusterMinCol,thisClusterSize,thisClusterRowSize,thisClusterColSize);
  if(verbosity_)std::cout << lp << " with error " << le << std::endl;

  hits.push_back(rh);
//+++++++++

  return;

}





