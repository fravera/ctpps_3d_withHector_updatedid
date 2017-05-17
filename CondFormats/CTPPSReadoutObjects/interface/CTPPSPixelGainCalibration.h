#ifndef CondFormats_CTPPSReadoutObjects_CTPPSPixelGainCalibration_h
#define CondFormats_CTPPSReadoutObjects_CTPPSPixelGainCalibration_h
// -*- C++ -*-
//
// Package:    CTPPSReadoutObjects
// Class:      CTPPSPixelGainCalibration
// 
/**\class CTPPSPixelGainCalibration CTPPSPixelGainCalibration.h CondFormats/CTPPSObjects/src/CTPPSPixelGainCalibration.cc

 Description: Gain calibration object for the CTPPS 3D Pixels.  Store gain/pedestal information at pixel granularity

 Implementation:
     <Notes on implementation>
*/
//  Based on SiPixelObjects/SiPixelGainCalibration 
// Original Author:  Vincenzo Chiochia
//         Created:  Tue 8 12:31:25 CEST 2007
//         Modified: Evan Friis
// $Id: CTPPSPixelGainCalibration.h,v 1.8 2009/02/10 17:25:42 rougny Exp $
//  Adapted for CTPPS : Clemencia Mora Herrera       November 2016
//
#include "CondFormats/Serialization/interface/Serializable.h"

#include<vector>
#include<map>
#include<iostream>
#include<boost/cstdint.hpp>

class CTPPSPixelGainCalibration {

 public:
  
  struct DetRegistry{ //to index the channels in each sensor 
    uint32_t detid;
    uint32_t ibegin;
    uint32_t iend;
    int ncols;
    
    COND_SERIALIZABLE;
  };
  
  /* class StrictWeakOrdering{ */
  /* public: */
  /*   bool operator() (const DetRegistry& p,const uint32_t& i) const {return p.detid < i;} */
  /* }; */
  
  /* typedef std::vector<float>::const_iterator                ContainerIterator;   */
  /* typedef std::pair<ContainerIterator, ContainerIterator>  Range;       */
  /* typedef std::vector<DetRegistry>                         Registry; */
  /* typedef Registry::const_iterator                         RegistryIterator; */
  
  // Constructors
  CTPPSPixelGainCalibration();
  CTPPSPixelGainCalibration(float minPed, float maxPed, float minGain, float maxGain);
  CTPPSPixelGainCalibration(const uint32_t& detid, const std::vector<float>& peds, const std::vector<float>& gains, 
			    float minPed=0., float maxPed=255., float minGain=0., float maxGain=255.);
  ~CTPPSPixelGainCalibration(){}

  void initialize(){}

  void setGainsPeds(const uint32_t& detId, const std::vector<float>& peds, const std::vector<float>& gains);
  /* bool  put(const uint32_t& detID,Range input, const int& nCols); */
  /* const Range getRange(const uint32_t& detID) const; */
  /* void  getDetIds(std::vector<uint32_t>& DetIds_) const; */
  /* const int getNCols(const uint32_t& detID) const; */
  /* const std::pair<const Range, const int> getRangeAndNCols(const uint32_t& detID) const; */

  //  unsigned int getNumberOfRowsToAverageOver() const { return numberOfRowsToAverageOver_; }
  double getGainLow() const { return minGain_; }
  double getGainHigh() const { return maxGain_; }
  double getPedLow() const { return minPed_; }
  double getPedHigh() const { return maxPed_; }

  // Set and get public methods


  void putData(uint32_t ipix, float ped, float gain);

  float getPed   (const int& col, const int& row) const;
  float getGain  (const int& col, const int& row) const;
  float getPed   (const uint32_t ipix)const;
  float getGain  (const uint32_t ipix)const;
  int   getNCols () const {return indexes.ncols;}
  int   getIEnd  () const {return indexes.iend;}
  private:

  /* float   encodeGain(const float& gain); */
  /* float   encodePed (const float& ped); */
  /* float   decodeGain(unsigned int gain) const; */
  /* float   decodePed (unsigned int ped) const; */
  void resetPixData(uint32_t ipix, float ped, float gain);

  std::vector<float> v_pedestals; 
  std::vector<float> v_gains;
  DetRegistry indexes;
  //I'd make it a single detRegistry w/ detID and collection of indices 
  float  minPed_, maxPed_, minGain_, maxGain_;


 COND_SERIALIZABLE;
};
    
#endif
