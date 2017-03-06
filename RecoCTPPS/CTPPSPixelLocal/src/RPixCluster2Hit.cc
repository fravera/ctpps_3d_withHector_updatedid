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

// get the miinimu pixel row/col
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

  std::cout << " INSIDE RPIXCluster2Hit::make_hit " << std::endl;
  std::cout << " hit pixels: " << std::endl;
  for(int i = 0; i < thisClusterSize; i++){

    std::cout <<aCluster.pixelRow(i)<< " "<< aCluster.pixelCol(i)<<" " << aCluster.pixelADC(i)<<" " << std::endl;

  } 

 
//temporary +++++
  LocalPoint lp(0,0,0);
  LocalError le(0,0,0);
  CTPPSPixelRecHit rh(lp,le,anEdgePixel,aBadPixel,twoRocs);
//+++++++++

  return;

}






/*

?????










// creating a set of CTPPSPixelDigi's and fill it
// NOTE: this should be also the place where SiPixels do the Calibration, i.e. when they create the buffer via make_buffer method!!
 rpix_digi_set_.clear();
 rpix_digi_set_.insert(digi.begin(),digi.end());


// try to modify/calibrate digis here
 calib_rpix_digi_set_.clear();
 for( std::set<CTPPSPixelDigi>::iterator RPdit = rpix_digi_set_.begin(); RPdit != rpix_digi_set_.end();++RPdit){
   int row = (*RPdit).row();
   int column = (*RPdit).column();
   int adc = (*RPdit).adc();
   int electrons = calibrate(adc,row,column);
//calibrate digi and store the new ones (it still does nothing!!)
   RPixCalibDigi calibDigi(row,column,adc,electrons);
   calib_rpix_digi_set_.insert(calibDigi);


 }
 if(verbosity_) std::cout<<" RPix set size = "<<calib_rpix_digi_set_.size()<<std::endl;
// storing the seeds
 SeedVector_.clear();
 std::set<RPixCalibDigi>::iterator RPDit;
 for(RPDit = calib_rpix_digi_set_.begin(); RPDit!=calib_rpix_digi_set_.end();++RPDit){
       if(verbosity_) std::cout<<"adc    " << (*RPDit).adc()<< std::endl;
       if(verbosity_) std::cout<<"ele    " << (*RPDit).electrons()<< std::endl;

   if((*RPDit).electrons() > SeedADCThreshold_*ElectronADCGain_){
     
   // storing the seed if above threshold
     SeedVector_.push_back(*RPDit);

   } 
 }
 if(verbosity_) std::cout<<" SeedVector size = "<<SeedVector_.size()<<std::endl;



//----
// Looping on the seeds to make a cluster around the seed

 for(std::vector<RPixCalibDigi>::iterator SeedIt = SeedVector_.begin(); SeedIt!=SeedVector_.end();++SeedIt){

// make cluster around the seed

   make_cluster( *SeedIt, clusters);




 }

}

void RPixCluster2Hit::make_cluster(RPixCalibDigi aSeed,  std::vector<CTPPSPixelCluster> &clusters ){

// THIS DEPENDS ON SENSOR TOPOLOGY!!!

// CHECK ALSO FOR CALIBRATION

  int max_rows_in_sensor=160;
  int max_cols_in_sensor=156;


// creating a temporary cluster

  tempCluster atempCluster;

// filling the cluster with the seed
 atempCluster.addPixel(aSeed.row(),aSeed.column(),aSeed.electrons());
calib_rpix_digi_set_.erase( aSeed );


 while ( ! atempCluster.empty()) {
   //This is the standard algorithm to find and add a pixel
     auto curInd = atempCluster.top(); atempCluster.pop();
     for ( auto c = std::max(0,int(atempCluster.col[curInd])-1); c < std::min(int(atempCluster.col[curInd])+2,max_cols_in_sensor) ; ++c) {
	 for ( auto r = std::max(0,int(atempCluster.row[curInd])-1); r < std::min(int(atempCluster.row[curInd])+2,max_rows_in_sensor); ++r)  {

//
	   for(std::set<RPixCalibDigi>::iterator RPDit4 = calib_rpix_digi_set_.begin(); RPDit4 != calib_rpix_digi_set_.end(); ){
	     if( (*RPDit4).column() == c && (*RPDit4).row() == r && (*RPDit4).electrons() > ADCThreshold_*ElectronADCGain_ ){
	     
	       if(!atempCluster.addPixel( r,c,(*RPDit4).electrons() )) {goto endClus;}
	       RPDit4 =  calib_rpix_digi_set_.erase(RPDit4);

	     }else{
	       ++RPDit4;
	     }

	   }

	 }
       }
	     
 }  // while accretion


  endClus:
     //  SiPixelCluster cluster(atempCluster.isize,atempCluster.adc, atempCluster.x,atempCluster.y, atempCluster.xmin,atempCluster.ymin);


  if(verbosity_) atempCluster.printCluster();
  CTPPSPixelCluster cluster(atempCluster.isize,atempCluster.adc, atempCluster.row,atempCluster.col, atempCluster.rowmin,atempCluster.colmin);
  clusters.push_back(cluster);



}
*/
/*
int RPixCluster2Hit::calibrate(int adc, int row, int col){

  const double gain = ElectronADCGain_;
  const double pedestal = 0;
//here DB values could be used instead

  int electrons = int(adc*gain + pedestal);

  return electrons;

}
*/


