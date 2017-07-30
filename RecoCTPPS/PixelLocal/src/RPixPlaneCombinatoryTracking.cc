#include "RecoCTPPS/PixelLocal/interface/RPixPlaneCombinatoryTracking.h"
#include <algorithm>

#include "TMatrixD.h"
#include "TVectorD.h"
#include <math.h>

//------------------------------------------------------------------------------------------------//

RPixPlaneCombinatoryTracking::RPixPlaneCombinatoryTracking(edm::ParameterSet const& parameterSet) : 
  RPixDetTrackFinder(parameterSet),
  param_(parameterSet){

  trackMinNumberOfPoints_ = param_.getUntrackedParameter<uint>("RPixTrackMinNumberOfPoints");
  verbosity_ = param_.getUntrackedParameter<int> ("RPixVerbosity");
  maximumChi2OverNDF_ = param_.getParameter<double>("MaximumChi2OverNDF");
  maximumChi2RelativeIncreasePerNDF_= param_.getParameter<double>("MaximumChi2RelativeIncreasePerNDF");

  if(trackMinNumberOfPoints_<3){
    throw cms::Exception("RPixPlaneCombinatoryTracking") << "Minimum number of planes required for tracking is 3, tracking is not possible with " << trackMinNumberOfPoints_ << " hits";
  }
  
  uint32_t numberOfPlanes = 6;
  for(uint32_t i=0; i<numberOfPlanes; ++i){
    CTPPSPixelDetId tmpDetId = romanPotId_; //to avoid modify the data member
    tmpDetId.setPlane(i);
    listOfAllPlanes_.push_back(tmpDetId);
  }


  uint32_t numberOfCombinations = Factorial(numberOfPlanes)/(Factorial(numberOfPlanes-trackMinNumberOfPoints_)*Factorial(trackMinNumberOfPoints_));
  if(verbosity_>2) std::cout<<"Number of combinations = "<<numberOfCombinations<<std::endl;
  possiblePlaneCombinations_.reserve(numberOfCombinations);

  possiblePlaneCombinations_ = getPlaneCombinations(listOfAllPlanes_,trackMinNumberOfPoints_);

  if(verbosity_>2) {
    for( const auto & vec : possiblePlaneCombinations_){
      for( const auto & num : vec){
        std::cout<<num<<" - ";
      }
      std::cout<<std::endl;
    }
  }

}

//------------------------------------------------------------------------------------------------//
    
RPixPlaneCombinatoryTracking::~RPixPlaneCombinatoryTracking() {
  possiblePlaneCombinations_.clear();
}

//------------------------------------------------------------------------------------------------//
    
//This function produces all the possible plane combination extracting numberToExtract planes over numberOfPlanes planes
std::vector<std::vector<CTPPSPixelDetId> > RPixPlaneCombinatoryTracking::getPlaneCombinations(std::vector<CTPPSPixelDetId> inputPlaneList, uint32_t numberToExtract)
{
    uint32_t numberOfPlanes = inputPlaneList.size();
    std::string bitmask(numberToExtract, 1); // numberToExtract leading 1's
    bitmask.resize(numberOfPlanes, 0); // numberOfPlanes-numberToExtract trailing 0's
    std::vector<std::vector<CTPPSPixelDetId> > planeCombinations;

    // store the combination and permute bitmask
    do {
      std::vector<CTPPSPixelDetId> tmpPlaneList;
      for (uint32_t i = 0; i < numberOfPlanes; ++i) { // [0..numberOfPlanes-1] integers
        if (bitmask[i]) tmpPlaneList.push_back(inputPlaneList.at(i));
      }
        planeCombinations.push_back(tmpPlaneList);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return planeCombinations;
}


//------------------------------------------------------------------------------------------------//

//This function calls it self in order to get all the possible combination of hits having selected a certain number of planes
//This function avoing to write for loops in cascade which will not allow to define arbitrarily the minimum number of planes to use
//The output is stored in a map containg the vector of points and as a key the map of the point forming this vector.
//This allows to erase the points already associated to a track
void RPixPlaneCombinatoryTracking::getHitCombinations(
  const std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> > &mapOfAllHits, 
  std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> >::iterator mapIterator,
  std::map<CTPPSPixelDetId, size_t> tmpHitPlaneMap,
  std::vector<RPixDetPatternFinder::PointAndRecHit> tmpHitVector,
  std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > &outputMap)
{
    //At this point I selected one hit per plane, so I can add
    if (mapIterator == mapOfAllHits.end())
    {
        outputMap[tmpHitPlaneMap] = tmpHitVector;
        return;
    }
    for (size_t i=0; i<mapIterator->second.size(); i++){
      std::map<CTPPSPixelDetId, size_t> newHitPlaneMap = tmpHitPlaneMap;
      newHitPlaneMap[mapIterator->first] = i;
      std::vector<RPixDetPatternFinder::PointAndRecHit> newVector = tmpHitVector;
      newVector.push_back(mapIterator->second.at(i));
      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> >::iterator tmpMapIterator = mapIterator;
      getHitCombinations(mapOfAllHits, ++tmpMapIterator, newHitPlaneMap, newVector, outputMap);
    }
}

//------------------------------------------------------------------------------------------------//

std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > RPixPlaneCombinatoryTracking::produceAllHitCombination(std::vector<std::vector<CTPPSPixelDetId> > inputPlaneCombination){
  
  std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > mapOfAllPoints;

  //Loop on all the plane combination
  for( const auto & planeCombination : inputPlaneCombination){

    std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> > selectedCombinationHitOnPlane;
    bool allPlaneAsHits = true;

    //Loop on all the possible combination
    //In this loop the selectedCombinationHitOnPlane is filled and cases with one the selected plane is empty are skipped
    for( const auto & plane : planeCombination){
      if(hitMap_.find(plane) == hitMap_.end()){
        allPlaneAsHits = false;
        break;
      }
      if(selectedCombinationHitOnPlane.find(plane)!=selectedCombinationHitOnPlane.end()){
           throw cms::Exception("RPixPlaneCombinatoryTracking") <<"selectedCombinationHitOnPlane contains already detId "<<plane<<std::endl<<
           "Error in the algorithm which created all the possible plane combinations";
      }
      selectedCombinationHitOnPlane[plane];
    }
    if(!allPlaneAsHits) break;

    //I add the all the hit combinations to the full list of plane combinations
    std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> >::iterator mapIterator=selectedCombinationHitOnPlane.begin();
    std::map<CTPPSPixelDetId, size_t> tmpHitPlaneMap; //empty map of plane and hit number needed the getHitCombinations algorithm
    std::vector<RPixDetPatternFinder::PointAndRecHit> tmpHitVector; //empty vector of hits needed for the getHitCombinations algorithm
    getHitCombinations(selectedCombinationHitOnPlane,mapIterator,tmpHitPlaneMap,tmpHitVector,mapOfAllPoints);

  } //end of loops on all the combinations

  return mapOfAllPoints;

}

//------------------------------------------------------------------------------------------------//


void RPixPlaneCombinatoryTracking::findTracks(){

  //The loop search for all the possible tracks starting from the one with the lowest chiSquare/NDF
  //The loop stops when the number of planes with hit is less than the minimum required
  //or if the track with minumin chiSquare found has a chiSquare higher than the maximum required
  while(hitMap_.size()<trackMinNumberOfPoints_){

    //I create the map of all the possible combination of a group of trackMinNumberOfPoints_ point
    //and the key keeps the reference of which plane and which hit number the forms the combination
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > mapOfAllMinRequiredPoint;
    //I produce the map for all cominatory of all hits with all trackMinNumberOfPoints_-planes combinations
    mapOfAllMinRequiredPoint =produceAllHitCombination(possiblePlaneCombinations_);



   /* 
    //Loop on all the possible trackMinNumberOfPoints_-planes combination
    for( const auto & planeCombination : possiblePlaneCombinations_){

      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> > selectedCombinationHitOnPlane;
      bool allPlaneAsHits = true;

      //Loop on all the possible combination
      //In this loop the selectedCombinationHitOnPlane is filled and cases with one the selected plane is empty are skipped
      for( const auto & plane : planeCombination){
        if(hitMap_.find(plane) == hitMap_.end()){
          allPlaneAsHits = false;
          break;
        }
        if(selectedCombinationHitOnPlane.find(plane)!=selectedCombinationHitOnPlane.end()){
             throw cms::Exception("RPixPlaneCombinatoryTracking") <<"selectedCombinationHitOnPlane contains already detId "<<plane<<endl<<
             "Error in the algorithm which created all the possible plane combinations"<<endl;
        }
        selectedCombinationHitOnPlane[plane];
      }
      if(!allPlaneAsHits) break;

      //I add the 4-hits combination of the present trackMinNumberOfPoints_-planes combination to the full list of combinations
      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> >::iterator mapIterator=selectedCombinationHitOnPlane.begin();
      std::map<CTPPSPixelDetId, size_t> tmpHitPlaneMap; //empty map of plane and hit number needed the getHitCombinations algorithm
      std::vector<RPixDetPatternFinder::PointAndRecHit> tmpHitVector; //empty vector of hits needed for the getHitCombinations algorithm
      getHitCombinations(selectedCombinationHitOnPlane,mapIterator,tmpHitPlaneMap,tmpHitVector,mapOfAllMinRequiredPoint);

    } //end of loops on all the combinations
    */

    //Fit all the possible combinations of minimum plane required and find the track with minimum chi2
    double theMinChiSquaredOverNDF = maximumChi2OverNDF_+1.; //in order to break the loop in case no track is found;
    std::map<CTPPSPixelDetId, size_t> pointMapWithMinChiSquared;
    std::vector<RPixDetPatternFinder::PointAndRecHit> pointsWithMinChiSquared;
    CTPPSPixelLocalTrack bestTrack;
    for(const auto & pointsAndRef : mapOfAllMinRequiredPoint){
      CTPPSPixelLocalTrack tmpTrack = fitTrack(pointsAndRef.second);
      double tmpChiSquaredOverNDF = tmpTrack.getChiSquaredOverNDF();
      if(!tmpTrack.isValid() || tmpChiSquaredOverNDF>maximumChi2OverNDF_ || tmpChiSquaredOverNDF==0.) continue; //validity check
      if(tmpChiSquaredOverNDF<theMinChiSquaredOverNDF){
        theMinChiSquaredOverNDF = tmpChiSquaredOverNDF;
        pointMapWithMinChiSquared = pointsAndRef.first;
        pointsWithMinChiSquared = pointsAndRef.second;
        bestTrack = tmpTrack;
      }
    }
    //The loop on the fit of all tracks is done, the track with minimum chiSquared is found
    // and it is verified that it complies with the maximumChi2OverNDF_ requirement
    if(theMinChiSquaredOverNDF > maximumChi2OverNDF_) break;

    //The list of planes not included in the minimum chiSquare track is produced.
    std::vector<CTPPSPixelDetId>  listOfExcludedPlanes;
    for(const auto & plane : listOfAllPlanes_){
      if(pointMapWithMinChiSquared.find(plane)==pointMapWithMinChiSquared.end()) listOfExcludedPlanes.push_back(plane);
    }

    //I produce the all possible combination of planes to be added to the track,
    //excluding the case of no other plnes added since it has been already fitted.
    std::vector<std::vector<CTPPSPixelDetId> > planePointedHitListCombination;
    for(uint32_t i=1; i<=listOfExcludedPlanes.size(); ++i){
      std::vector<std::vector<CTPPSPixelDetId> > tmpPlaneCombination = getPlaneCombinations(listOfExcludedPlanes,i);
      for(const auto & combination : tmpPlaneCombination) planePointedHitListCombination.push_back(combination);
    }

    //I produce all the possible combination of points to be added to the track
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > mapOfAllPointWithAtLeastBestFitSelected;
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointAndRecHit> > mapOfPointCombinationToBeAdded;
    mapOfPointCombinationToBeAdded = produceAllHitCombination(planePointedHitListCombination);
    //The found hit combination is added to the hits selected by the best fit;
    for(const auto & element : mapOfPointCombinationToBeAdded){
      std::map<CTPPSPixelDetId, size_t> newPointMap = pointMapWithMinChiSquared; 
      std::vector<RPixDetPatternFinder::PointAndRecHit> newPoints = pointsWithMinChiSquared;
      for(const auto & pointRef : element.first ) newPointMap[pointRef.first] = pointRef.second; //add the new point reference
      for(const auto & point    : element.second) newPoints.push_back(point);
      mapOfAllPointWithAtLeastBestFitSelected[newPointMap]=newPoints;
    }
  
    //I fit all the possible combination of the minimum plane best fit hits plus hits from the other planes
    for(const auto & pointsAndRef : mapOfAllPointWithAtLeastBestFitSelected){
      CTPPSPixelLocalTrack tmpTrack = fitTrack(pointsAndRef.second);
      double tmpChiSquaredOverNDF = tmpTrack.getChiSquaredOverNDF();
      if(!tmpTrack.isValid() || tmpChiSquaredOverNDF>maximumChi2OverNDF_ || tmpChiSquaredOverNDF==0.) continue; //validity check
      double ChiSquaredOverNDFCorrectionFactor = pow(1.-maximumChi2RelativeIncreasePerNDF_,pointsAndRef.second.size()-trackMinNumberOfPoints_);
      //it keeps into account that an higher chiSquare is tolerated if more hits are included in the fit
      //cases in which the correction factor would allow to accept chiSquare higher than maximumChi2OverNDF_
      //are already considered in the validity check
      tmpChiSquaredOverNDF*=ChiSquaredOverNDFCorrectionFactor; 
      if(tmpChiSquaredOverNDF<theMinChiSquaredOverNDF){
        theMinChiSquaredOverNDF = tmpChiSquaredOverNDF;
        pointMapWithMinChiSquared = pointsAndRef.first;
        pointsWithMinChiSquared = pointsAndRef.second;
        bestTrack = tmpTrack;
      }
    }

    localTrackVector_.push_back(bestTrack);

    //remove the hits belonging to the tracks from the full list of hits
    for(const auto & hitToErase : pointMapWithMinChiSquared){
      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointAndRecHit> >::iterator hitMapElement = hitMap_.find(hitToErase.first);
      if(hitMapElement==hitMap_.end()){
           throw cms::Exception("RPixPlaneCombinatoryTracking") <<"The found tracks has hit belonging to a plane which does not have hits";
      }
      hitMapElement->second.erase(hitMapElement->second.begin()+hitToErase.second);
      //if the plane at which the hit was erased ie empty it is removed from the hit map
      if(hitMapElement->second.size() == 0) hitMap_.erase(hitMapElement);
    }


  }

  return;

}

CTPPSPixelLocalTrack RPixPlaneCombinatoryTracking::fitTrack(std::vector<RPixDetPatternFinder::PointAndRecHit> pointList){
  return CTPPSPixelLocalTrack();
}

