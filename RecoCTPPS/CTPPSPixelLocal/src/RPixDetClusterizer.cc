#include <iostream>
#include "FWCore/Utilities/interface/Exception.h"

#include "RecoCTPPS/CTPPSPixelLocal/interface/RPixDetClusterizer.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"

namespace {
  constexpr int max16bits = 65535;
  constexpr int maxCol = 155;
  constexpr int maxRow = 159;
}


RPixDetClusterizer::RPixDetClusterizer(edm::ParameterSet const& conf):
  params_(conf), SeedVector_(0)
{
verbosity_ = conf.getParameter<int>("RPixVerbosity");
SeedADCThreshold_ = conf.getParameter<int>("SeedADCThreshold");
ADCThreshold_ = conf.getParameter<int>("ADCThreshold");
ElectronADCGain_ = conf.getParameter<double>("ElectronADCGain");
VcaltoElectronGain_ = conf.getParameter<int>("VCaltoElectronGain");
VcaltoElectronOffset_ = conf.getParameter<int>("VCaltoElectronOffset");
<<<<<<< HEAD
DAQCalibration_ = conf.getParameter<bool>("DAQCalibration");
CalibrationFile_ = conf.getParameter<string>("CalibrationFile");
theDAQcalibration =  new CTPPSPixelDAQCalibration(conf);
theDAQcalibration->setDAQCalibrationFile(CalibrationFile_);
=======
doSingleCalibration_ = conf.getParameter<bool>("doSingleCalibration");
>>>>>>> helio/DB910pre3
}

RPixDetClusterizer::~RPixDetClusterizer(){}

void RPixDetClusterizer::buildClusters(unsigned int detId, const std::vector<CTPPSPixelDigi> &digi, std::vector<CTPPSPixelCluster> &clusters, const CTPPSPixelGainCalibrations * pcalibrations)
{

  if(verbosity_) std::cout<<" RPixDetClusterizer "<<detId<<" received digi.size()="<<digi.size()<<std::endl;
  if(verbosity_)  for(unsigned int i=0; i<digi.size();i++)std::cout<< digi[i].adc() << std::endl;


// creating a set of CTPPSPixelDigi's and fill it
// NOTE: this should be also the place where SiPixels do the Calibration, i.e. when they create the buffer via make_buffer method!!
  rpix_digi_set_.clear();
  rpix_digi_set_.insert(digi.begin(),digi.end());


// try to modify/calibrate digis here
  calib_rpix_digi_set_.clear();
  for( std::set<CTPPSPixelDigi>::iterator RPdit = rpix_digi_set_.begin(); RPdit != rpix_digi_set_.end();++RPdit){

    if((*RPdit).row() > maxRow || (*RPdit).column() > maxCol)
       throw cms::Exception("CTPPSDigiOutofRange") 
	 << " row = " << (*RPdit).row() << "  column = "<< (*RPdit).column();
    std::cout << " row = " << (*RPdit).row() << "  column = "<< (*RPdit).column() << "  adc = " << (*RPdit).adc() << std::endl;
    unsigned char row = (*RPdit).row();
    unsigned char column = (*RPdit).column();
    unsigned short adc = (*RPdit).adc();
    unsigned short electrons = calibrate(detId,adc,row,column,pcalibrations);
//calibrate digi and store the new ones
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
   
// testing find method ... to be removed
//   std::set<RPixCalibDigi>::iterator RPDit3 = calib_rpix_digi_set_.find( *SeedIt );
//   if(verbosity_) std::cout<<"    " <<  (*RPDit3).adc()<< std::endl;

// make cluster around the seed

    make_cluster( *SeedIt, clusters);

// to be finished ........


  }

}

void RPixDetClusterizer::make_cluster(RPixCalibDigi aSeed,  std::vector<CTPPSPixelCluster> &clusters ){

// THIS DEPENDS ON SENSOR TOPOLOGY!!!

// CHECK ALSO FOR CALIBRATION

//  int max_rows_in_sensor=160;
//  int max_cols_in_sensor=156;

/// check if seed already used
  if(calib_rpix_digi_set_.size()==0 || calib_rpix_digi_set_.find(aSeed)==calib_rpix_digi_set_.end() ){
    return;
  }


// creating a temporary cluster

  tempCluster atempCluster;

// filling the cluster with the seed
  atempCluster.addPixel(aSeed.row(),aSeed.column(),aSeed.electrons());
  calib_rpix_digi_set_.erase( aSeed );


  while ( ! atempCluster.empty()) {
  //This is the standard algorithm to find and add a pixel
    auto curInd = atempCluster.top(); atempCluster.pop();
    for ( auto c = std::max(0,int(atempCluster.col[curInd])-1); c < std::min(int(atempCluster.col[curInd])+2,maxCol) ; ++c) {
      for ( auto r = std::max(0,int(atempCluster.row[curInd])-1); r < std::min(int(atempCluster.row[curInd])+2,maxRow); ++r)  {

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

//  if(cluster.minPixelRow()>130)std::cout << "cluster.minPixelRow >130 "<< uint8_t(atempCluster.rowmin) << " " <<uint8_t(cluster.minPixelRow() )<<std::endl;;

// SPLITTING LARGE CLUSTERS TO BE DONE

}

int RPixDetClusterizer::calibrate(unsigned int detId, int adc, int row, int col, const CTPPSPixelGainCalibrations *pcalibrations){

  bool isnoisy_g=false;  
  bool isdead_g=false;
  bool isnoisy_p=false;
  bool isdead_p=false;
  float gain=0;
  float pedestal=0;
<<<<<<< HEAD
// double -> float

  if(DAQCalibration_){
    theDAQcalibration->getDAQCalibration(detId,row,col,gain,pedestal);
  }else{

    detId = 2014314496; //just one plane on test DB file 

    CTPPSPixelGainCalibration DetCalibs = pcalibrations->getGainCalibration(detId);
    gain = DetCalibs.getGain(col,row,isdead_g,isnoisy_g);
    pedestal = DetCalibs.getPed(col,row,isdead_p,isnoisy_p)*gain;
  }
  float vcal = adc*gain - pedestal;
  int electrons = int(vcal*VcaltoElectronGain_ + VcaltoElectronOffset_);

=======
  float electrons=0;
// double -> float

  detId = 2014314496; //just one plane in test DB file 

  if(!doSingleCalibration_){
    CTPPSPixelGainCalibration DetCalibs = pcalibrations->getGainCalibration(detId);
    gain = DetCalibs.getGain(col,row,isdead_g,isnoisy_g);
    pedestal = DetCalibs.getPed(col,row,isdead_p,isnoisy_p);
    float vcal = (adc - pedestal)*gain;
    electrons = int(vcal*VcaltoElectronGain_ + VcaltoElectronOffset_);
    if(verbosity_) std::cout << "calibrate:  adc = " << adc << " electrons = " << electrons << " gain = " << gain << " pedestal = " << pedestal << endl;
  }else{
    gain = ElectronADCGain_;
    pedestal = 0;
    electrons = int(adc*gain-pedestal);
    if(verbosity_) std::cout << "calibrate:  adc = " << adc << " electrons = " << electrons << " ElectronADCGain = " << gain << " pedestal = " << pedestal << endl;
  }
  if (electrons<0) {
    std::cout << "RPixDetClusterizer::calibrate: *** electrons < 0 *** --> " << electrons << "  --> electrons = 0" << endl;
    electrons = 0;
  }
>>>>>>> helio/DB910pre3
  return electrons;

}
