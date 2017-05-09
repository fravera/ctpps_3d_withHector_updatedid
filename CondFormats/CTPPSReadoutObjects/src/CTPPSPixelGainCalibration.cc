#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelGainCalibration.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <algorithm>
#include <cstring>

//
// Constructors
//
CTPPSPixelGainCalibration::CTPPSPixelGainCalibration() :
  minPed_(0.),
  maxPed_(255.),
  minGain_(0.),
  maxGain_(255.) //,
  //  numberOfRowsToAverageOver_(1),
  //  nBinsToUseForEncoding_(253),
  //  deadFlag_(255),
  //  noisyFlag_(254)
{
   // if (deadFlag_ > 0xFF)
   //    throw cms::Exception("GainCalibration Payload configuration error")
   //       << "[CTPPSPixelGainCalibration::CTPPSPixelGainCalibration] Dead flag was set to " << deadFlag_ << ", and it must be set less than or equal to 255";
  std::cout<<"Default instance of CTPPSPixelGainCalibration"<<std::endl;
}

//
CTPPSPixelGainCalibration::CTPPSPixelGainCalibration(float minPed, float maxPed, float minGain, float maxGain) :
  minPed_(minPed),
  maxPed_(maxPed),
  minGain_(minGain),
  maxGain_(maxGain)  //,
  //  numberOfRowsToAverageOver_(1),
  //  nBinsToUseForEncoding_(253),
  //  deadFlag_(255),
  //  noisyFlag_(254)
{
   // if (deadFlag_ > 0xFF)
   //    throw cms::Exception("GainCalibration Payload configuration error")
   //       << "[CTPPSPixelGainCalibration::CTPPSPixelGainCalibration] Dead flag was set to " << deadFlag_ << ", and it must be set less than or equal to 255";
  std::cout<<"Instance of CTPPSPixelGainCalibration setting mininums and maximums"<<std::endl;
}
//

CTPPSPixelGainCalibration::CTPPSPixelGainCalibration(const uint32_t& detid, const std::vector<float> & peds, const std::vector<float>& gains, float minPed, float maxPed,float minGain, float maxGain) :
  minPed_(minPed),
  maxPed_(maxPed),
  minGain_(minGain),
  maxGain_(maxGain) //,
  // deadFlag_(255),
  //  noisyFlag_(254)
{
   // if (deadFlag_ > 0xFF)
   //    throw cms::Exception("GainCalibration Payload configuration error")
   //       << "[CTPPSPixelGainCalibration::CTPPSPixelGainCalibration] Dead flag was set to " << deadFlag_ << ", and it must be set less than or equal to 255";

  std::cout<<"Instance of CTPPSPixelGainCalibration setting peds and gains"<<std::endl;
  setGainsPeds(detid,peds,gains);
}

void CTPPSPixelGainCalibration::setGainsPeds(const uint32_t& detid, const std::vector<float> & peds, const std::vector<float>& gains){
  int sensorSize=peds.size();
  int gainsSize=gains.size();
  if(gainsSize!=sensorSize)
    throw cms::Exception("CTPPSPixelGainCalibration payload configuration error") 
      <<  "[CTPPSPixelGainCalibration::CTPPSPixelGainCalibration]  peds and gains vector sizes don't match for detid "
      << detid << " size peds = " << sensorSize << " size gains = " << gainsSize; 
  DetRegistry myreg;
  myreg.detid=detid;
  myreg.ibegin=0;
  myreg.iend=sensorSize;
  myreg.ncols=sensorSize/160; // each ROC is made of 80 rows and 52 columns, each sensor is made of 160 rows and either 104 or 156 columns (2x2 or 2x3 ROCs)
  indexes = myreg;
  for(int i = 0 ; i<sensorSize ; ++i)  
    putData(i,peds[i],gains[i]);
}

void CTPPSPixelGainCalibration::putData(uint32_t ipix, float ped, float gain){
  if (v_pedestals.size()>ipix) //the vector is too big, this pixel has already been set
    {
      if (ped>=0 && gain>=0)
	throw cms::Exception("CTPPSPixelGainCalibration fill error")
	  << "[CTPPSPixelGainCalibration::putData] attemptint to fill the vectors that are already filled detid = " << indexes.detid << " ipix " << ipix;
      else   // in case it is for setting the noisy or dead flag of already filled pixel
	{
	  std::cout<<"resetting pixel calibration for noisy or dead flag"<<std::endl;
	  resetPixData(ipix,ped,gain);
	}
    }
  else if (v_pedestals.size()<ipix)
    throw cms::Exception("CTPPSPixelGainCalibration fill error")
      << "[CTPPSPixelGainCalibration::putData] the order got scrambled detid = "<< indexes.detid << " ipix " << ipix;
  else{ //the size has to match exactly the index, the index - 0 pixel starts the vector, the one with index 1 should be pushed back in a vector of size== 1 (to become size==2) so on and o forth
    v_pedestals.push_back(ped);
    v_gains.push_back(gain);
  }
}
  
void CTPPSPixelGainCalibration::resetPixData(uint32_t ipix, float ped, float gain){
  if (v_pedestals.size()<=ipix){
    std::cout<<"ERROR here this element has not been set yet"<<std::endl; return;
  }
  v_pedestals[ipix]=ped; //modify value to already exisiting element
  v_gains[ipix]=gain;
}


// bool CTPPSPixelGainCalibration::put(const uint32_t& DetId, Range input, const int& nCols) {
//   // put in CTPPSPixelGainCalibration of DetId

//   Registry::iterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,CTPPSPixelGainCalibration::StrictWeakOrdering());
//   if (p!=indexes.end() && p->detid==DetId)
//     return false;
  
//   size_t sd= input.second-input.first;
//   DetRegistry detregistry;
//   detregistry.detid=DetId;
//   detregistry.ncols=nCols;
//   detregistry.ibegin=v_pedestals.size();
//   detregistry.iend=v_pedestals.size()+sd;
//   indexes.insert(p,detregistry);

//   v_pedestals.insert(v_pedestals.end(),input.first,input.second);
//   return true;
// }

// const int CTPPSPixelGainCalibration::getNCols(const uint32_t& DetId) const {
//   // get number of columns of DetId
//   RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,CTPPSPixelGainCalibration::StrictWeakOrdering());
//   if (p==indexes.end()|| p->detid!=DetId) 
//     return 0;
//   else
//     return p->ncols; 
// }

// const CTPPSPixelGainCalibration::Range CTPPSPixelGainCalibration::getRange(const uint32_t& DetId) const {
//   // get CTPPSPixelGainCalibration Range of DetId
  
//   RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,CTPPSPixelGainCalibration::StrictWeakOrdering());
//   if (p==indexes.end()|| p->detid!=DetId) 
//     return CTPPSPixelGainCalibration::Range(v_pedestals.end(),v_pedestals.end()); 
//   else 
//     return CTPPSPixelGainCalibration::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend);
// }

// const std::pair<const CTPPSPixelGainCalibration::Range, const int>
// CTPPSPixelGainCalibration::getRangeAndNCols(const uint32_t& DetId) const {
//   RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,CTPPSPixelGainCalibration::StrictWeakOrdering());
//   if (p==indexes.end()|| p->detid!=DetId) 
//     return std::make_pair(CTPPSPixelGainCalibration::Range(v_pedestals.end(),v_pedestals.end()), 0); 
//   else 
//     return std::make_pair(CTPPSPixelGainCalibration::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend), p->ncols);
// }
  

// void CTPPSPixelGainCalibration::getDetIds(std::vector<uint32_t>& DetIds_) const {
//   // returns vector of DetIds in map
//   CTPPSPixelGainCalibration::RegistryIterator begin = indexes.begin();
//   CTPPSPixelGainCalibration::RegistryIterator end   = indexes.end();
//   for (CTPPSPixelGainCalibration::RegistryIterator p=begin; p != end; ++p) {
//     DetIds_.push_back(p->detid);
//   }
// }




// void CTPPSPixelGainCalibration::setData(float ped, float gain, std::vector<char>& vped, bool isDeadPixel, bool isNoisyPixel){
  
//   float theEncodedGain=0;
//   float theEncodedPed=0;
//   if(!isDeadPixel && !isNoisyPixel){
//     theEncodedGain = encodeGain(gain);
//     theEncodedPed  = encodePed (ped);
//   }

//   unsigned int deadFlag_=254;
//   unsigned int noisyFlag_=255;
//   unsigned int ped_   = (static_cast<unsigned int>(theEncodedPed))  & 0xFF; 
//   unsigned int gain_  = (static_cast<unsigned int>(theEncodedGain)) & 0xFF;

//   if (isDeadPixel)
//   {
//      ped_  = deadFlag_ & 0xFF;
//      gain_ = deadFlag_ & 0xFF;
//   }
//   if (isNoisyPixel)
//   {
//      ped_  = noisyFlag_ & 0xFF;
//      gain_ = noisyFlag_ & 0xFF;
//   }
//   unsigned int data = (ped_ << 8) | gain_ ;
//   vped.resize(vped.size()+2);
//   // insert in vector of char
//   ::memcpy((void*)(&vped[vped.size()-2]),(void*)(&data),2);
// }

float CTPPSPixelGainCalibration::getPed(const int& col, const int& row /*, const Range& range, const int& nCols*/, bool& isDead, bool& isNoisy) const {

  //  int nRows = (range.second-range.first)/2 / nCols;
  //const DecodingStructure & s = (const DecodingStructure & ) *(range.first+(col*nRows + row)*2);
  int nCols=indexes.ncols;
  int nRows=v_pedestals.size()/nCols; // should be =160
  if (col >= nCols || row >= nRows){
    throw cms::Exception("CorruptedData")
      << "[CTPPSPixelGainCalibration::getPed] Pixel out of range: col " << col << " row " << row;
  }  
  int ipix = col + row * nCols;
  return   getPed(ipix,isDead,isNoisy);
}


float CTPPSPixelGainCalibration::getPed(const uint32_t ipix,bool& isDead, bool&isNoisy) const {

  if (v_pedestals[ipix] == -999.0)
    isDead = true;  
  if (v_gains[ipix] == -9999.0)
    isNoisy = true;
  return v_pedestals[ipix];  

}




float CTPPSPixelGainCalibration::getGain(const int& col, const int& row /*, const Range& range, const int& nCols*/, bool& isDead, bool& isNoisy) const {

  //  int nRows = (range.second-range.first)/2 / nCols;
  //  const DecodingStructure & s = (const DecodingStructure & ) *(range.first+(col*nRows + row)*2);
  int nCols=indexes.ncols;
  int nRows=v_pedestals.size()/nCols; // should be 160
  if (col >= nCols || row >= nRows){
    throw cms::Exception("CorruptedData")
      << "[CTPPSPixelGainCalibration::getPed] Pixel out of range: col " << col << " row " << row;
  }  
  int ipix = col + row * nCols;
  return getGain(ipix,isDead,isNoisy);
}

float CTPPSPixelGainCalibration::getGain(const uint32_t ipix,bool& isDead, bool&isNoisy) const {

  if (v_pedestals[ipix] == -999. )
     isDead = true;  
  if (v_gains[ipix] == -9999.)
     isNoisy = true;
  return v_gains[ipix];

}



// float CTPPSPixelGainCalibration::encodeGain( const float& gain ) {
  
//   if(gain < minGain_ || gain > maxGain_ ) {
//     throw cms::Exception("InsertFailure")
//       << "[CTPPSPixelGainCalibration::encodeGain] Trying to encode gain (" << gain << ") out of range [" << minGain_ << "," << maxGain_ << "]\n";
//   } else {
//     double precision   = (maxGain_-minGain_)/static_cast<float>(nBinsToUseForEncoding_);
//     float  encodedGain = (float)((gain-minGain_)/precision);
//     return encodedGain;
//   }

// }

// float CTPPSPixelGainCalibration::encodePed( const float& ped ) {

//   if(ped < minPed_ || ped > maxPed_ ) {
//     throw cms::Exception("InsertFailure")
//       << "[CTPPSPixelGainCalibration::encodePed] Trying to encode pedestal (" << ped << ") out of range [" << minPed_ << "," << maxPed_ << "]\n";
//   } else {
//     double precision   = (maxPed_-minPed_)/static_cast<float>(nBinsToUseForEncoding_);
//     float  encodedPed = (float)((ped-minPed_)/precision);
//     return encodedPed;
//   }

// }

// float CTPPSPixelGainCalibration::decodePed( unsigned int ped ) const {

//   double precision = (maxPed_-minPed_)/static_cast<float>(nBinsToUseForEncoding_);
//   float decodedPed = (float)(ped*precision + minPed_);
//   return decodedPed;

// }

// float CTPPSPixelGainCalibration::decodeGain( unsigned int gain ) const {

//   double precision = (maxGain_-minGain_)/static_cast<float>(nBinsToUseForEncoding_);
//   float decodedGain = (float)(gain*precision + minGain_);
//   return decodedGain;

// }

