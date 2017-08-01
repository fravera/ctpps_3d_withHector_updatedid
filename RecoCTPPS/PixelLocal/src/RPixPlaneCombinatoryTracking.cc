#include "RecoCTPPS/PixelLocal/interface/RPixPlaneCombinatoryTracking.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <algorithm>

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "TMath.h"
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
    listOfAllPlanes_.push_back(i);
  }


  uint32_t numberOfCombinations = Factorial(numberOfPlanes)/(Factorial(numberOfPlanes-trackMinNumberOfPoints_)*Factorial(trackMinNumberOfPoints_));
  if(verbosity_>=2) std::cout<<"Number of combinations = "<<numberOfCombinations<<std::endl;
  possiblePlaneCombinations_.reserve(numberOfCombinations);

  possiblePlaneCombinations_ = getPlaneCombinations(listOfAllPlanes_,trackMinNumberOfPoints_);

  if(verbosity_>=2) {
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
std::vector<std::vector<uint32_t> > RPixPlaneCombinatoryTracking::getPlaneCombinations(std::vector<uint32_t> inputPlaneList, uint32_t numberToExtract)
{
    uint32_t numberOfPlanes = inputPlaneList.size();
    std::string bitmask(numberToExtract, 1); // numberToExtract leading 1's
    bitmask.resize(numberOfPlanes, 0); // numberOfPlanes-numberToExtract trailing 0's
    std::vector<std::vector<uint32_t> > planeCombinations;

    // store the combination and permute bitmask
    do {
      std::vector<uint32_t> tmpPlaneList;
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
  const std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> > &mapOfAllHits, 
  std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> >::iterator mapIterator,
  std::map<CTPPSPixelDetId, size_t> tmpHitPlaneMap,
  std::vector<RPixDetPatternFinder::PointInPlane> tmpHitVector,
  std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > &outputMap)
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
      std::vector<RPixDetPatternFinder::PointInPlane> newVector = tmpHitVector;
      newVector.push_back(mapIterator->second.at(i));
      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> >::iterator tmpMapIterator = mapIterator;
      getHitCombinations(mapOfAllHits, ++tmpMapIterator, newHitPlaneMap, newVector, outputMap);
    }
}

//------------------------------------------------------------------------------------------------//

std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > RPixPlaneCombinatoryTracking::produceAllHitCombination(std::vector<std::vector<uint32_t> > inputPlaneCombination){
  
  std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > mapOfAllPoints;
  CTPPSPixelDetId tmpRpIt = romanPotId_; //in order to avoid to modify the data member
  
  if(verbosity_>2) std::cout<<"Searching for all combinations..."<<std::endl;
  //Loop on all the plane combination
  for( const auto & planeCombination : inputPlaneCombination){

    std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> > selectedCombinationHitOnPlane;
    bool allPlaneAsHits = true;

    //Loop on all the possible combination
    //In this loop the selectedCombinationHitOnPlane is filled and cases with one the selected plane is empty are skipped
    for( const auto & plane : planeCombination){
      tmpRpIt.setPlane(plane);
      CTPPSPixelDetId planeDetId = tmpRpIt;
      if(hitMap_.find(planeDetId) == hitMap_.end()){
        if(verbosity_>2) std::cout<<"No data on arm "<<planeDetId.arm()<<" station "<<planeDetId.station()<<" rp " <<planeDetId.rp()<<" plane "<<planeDetId.plane()<<std::endl;
        allPlaneAsHits = false;

        break;
      }
      if(selectedCombinationHitOnPlane.find(planeDetId)!=selectedCombinationHitOnPlane.end()){
           throw cms::Exception("RPixPlaneCombinatoryTracking") <<"selectedCombinationHitOnPlane contains already detId "<<planeDetId<<std::endl<<
           "Error in the algorithm which created all the possible plane combinations";
      }
      selectedCombinationHitOnPlane[planeDetId] = hitMap_[planeDetId];
    }
    if(!allPlaneAsHits) break;

    //I add the all the hit combinations to the full list of plane combinations
    std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> >::iterator mapIterator=selectedCombinationHitOnPlane.begin();
    std::map<CTPPSPixelDetId, size_t> tmpHitPlaneMap; //empty map of plane and hit number needed the getHitCombinations algorithm
    std::vector<RPixDetPatternFinder::PointInPlane> tmpHitVector; //empty vector of hits needed for the getHitCombinations algorithm
    getHitCombinations(selectedCombinationHitOnPlane,mapIterator,tmpHitPlaneMap,tmpHitVector,mapOfAllPoints);
    if(verbosity_>2) std::cout<<"Number of possible tracks "<<mapOfAllPoints.size()<<std::endl;

  } //end of loops on all the combinations

  return mapOfAllPoints;

}

//------------------------------------------------------------------------------------------------//


void RPixPlaneCombinatoryTracking::findTracks(){

  //The loop search for all the possible tracks starting from the one with the lowest chiSquare/NDF
  //The loop stops when the number of planes with hit is less than the minimum required
  //or if the track with minumin chiSquare found has a chiSquare higher than the maximum required

  while(hitMap_.size()>trackMinNumberOfPoints_){

    //if(verbosity_>=1){
      //std::cout<<"Number of plane with hits "<<hitMap_.size()<<std::endl;
      if(verbosity_>=2) for(const auto & plane : hitMap_) std::cout<<"\tarm "<<plane.first.arm()<<" station "<<plane.first.station()<<" rp " <<plane.first.rp()<<" plane "<<plane.first.plane()<<" : "<<plane.second.size()<<std::endl;
    //}

    //I create the map of all the possible combination of a group of trackMinNumberOfPoints_ point
    //and the key keeps the reference of which plane and which hit number the forms the combination
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > mapOfAllMinRequiredPoint;
    //I produce the map for all cominatory of all hits with all trackMinNumberOfPoints_-planes combinations
    mapOfAllMinRequiredPoint =produceAllHitCombination(possiblePlaneCombinations_);

    //Fit all the possible combinations of minimum plane required and find the track with minimum chi2
    double theMinChiSquaredOverNDF = maximumChi2OverNDF_+1.; //in order to break the loop in case no track is found;
    std::map<CTPPSPixelDetId, size_t> pointMapWithMinChiSquared;
    std::vector<RPixDetPatternFinder::PointInPlane> pointsWithMinChiSquared;
    CTPPSPixelLocalTrack bestTrack;

    //if(verbosity_>=2) 
    if(verbosity_>2) std::cout<<"Number of combinations of trackMinNumberOfPoints_ planes "<<mapOfAllMinRequiredPoint.size()<<std::endl;
    for(const auto & pointsAndRef : mapOfAllMinRequiredPoint){
      CTPPSPixelLocalTrack tmpTrack = fitTrack(pointsAndRef.second);
      double tmpChiSquaredOverNDF = tmpTrack.getChiSquaredOverNDF();
      if(verbosity_>=2) std::cout<<"ChiSquare of the present track "<<tmpChiSquaredOverNDF<<std::endl;
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
    if(verbosity_>=1) std::cout<<"Minimum chiSquare over NDF for all the tracks "<<theMinChiSquaredOverNDF<<std::endl;
    if(theMinChiSquaredOverNDF > maximumChi2OverNDF_) break;

    //The list of planes not included in the minimum chiSquare track is produced.
    std::vector<uint32_t>  listOfExcludedPlanes;
    for(const auto & plane : listOfAllPlanes_){
      CTPPSPixelDetId tmpRpIt = romanPotId_; //in order to avoid to modify the data member
      tmpRpIt.setPlane(plane);
      CTPPSPixelDetId planeDetId = tmpRpIt;
      if(pointMapWithMinChiSquared.find(planeDetId)==pointMapWithMinChiSquared.end()) listOfExcludedPlanes.push_back(plane);
    }

    //I produce the all possible combination of planes to be added to the track,
    //excluding the case of no other plnes added since it has been already fitted.
    std::vector<std::vector<uint32_t> > planePointedHitListCombination;
    for(uint32_t i=1; i<=listOfExcludedPlanes.size(); ++i){
      std::vector<std::vector<uint32_t> > tmpPlaneCombination = getPlaneCombinations(listOfExcludedPlanes,i);
      for(const auto & combination : tmpPlaneCombination) planePointedHitListCombination.push_back(combination);
    }

    //I produce all the possible combination of points to be added to the track
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > mapOfAllPointWithAtLeastBestFitSelected;
    std::map< std::map<CTPPSPixelDetId, size_t>, std::vector<RPixDetPatternFinder::PointInPlane> > mapOfPointCombinationToBeAdded;
    mapOfPointCombinationToBeAdded = produceAllHitCombination(planePointedHitListCombination);
    //The found hit combination is added to the hits selected by the best fit;
    for(const auto & element : mapOfPointCombinationToBeAdded){
      std::map<CTPPSPixelDetId, size_t> newPointMap = pointMapWithMinChiSquared; 
      std::vector<RPixDetPatternFinder::PointInPlane> newPoints = pointsWithMinChiSquared;
      for(const auto & pointRef : element.first ) newPointMap[pointRef.first] = pointRef.second; //add the new point reference
      for(const auto & point    : element.second) newPoints.push_back(point);
      mapOfAllPointWithAtLeastBestFitSelected[newPointMap]=newPoints;
    }
  
    //I fit all the possible combination of the minimum plane best fit hits plus hits from the other planes
    for(const auto & pointsAndRef : mapOfAllPointWithAtLeastBestFitSelected){
      CTPPSPixelLocalTrack tmpTrack = fitTrack(pointsAndRef.second);
      double tmpChiSquaredOverNDF = tmpTrack.getChiSquaredOverNDF();
      if(!tmpTrack.isValid() || tmpChiSquaredOverNDF>maximumChi2OverNDF_ || tmpChiSquaredOverNDF==0.) continue; //validity check
      double ChiSquaredOverNDFCorrectionFactor = TMath::Power(1.-maximumChi2RelativeIncreasePerNDF_,(int)(pointsAndRef.second.size()-trackMinNumberOfPoints_));
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
      std::map<CTPPSPixelDetId, std::vector<RPixDetPatternFinder::PointInPlane> >::iterator hitMapElement = hitMap_.find(hitToErase.first);
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

CTPPSPixelLocalTrack RPixPlaneCombinatoryTracking::fitTrack(std::vector<RPixDetPatternFinder::PointInPlane> pointList){
  
  uint32_t numberOfPoints = pointList.size();
  TVectorD xyCoordinates(2*numberOfPoints);
  TMatrixD varianceMatrix(2*numberOfPoints,2*numberOfPoints);
  TMatrixD zMatrix(2*numberOfPoints,4);
  
  double z0 = 0.;

  //The matrices and vector xyCoordinates, varianceMatrix and varianceMatrix are built from the points
  uint32_t hitCounter=0;
  for(const auto & hit : pointList){
    CLHEP::Hep3Vector globalPoint = hit.globalPoint;
    xyCoordinates[hitCounter]     = globalPoint.x()   ;
    xyCoordinates[hitCounter+1]   = globalPoint.y()   ;
    zMatrix      [hitCounter][0]   =                 1.;
    zMatrix      [hitCounter][2]   = globalPoint.z()-z0;
    zMatrix      [hitCounter+1][1] =                 1.;
    zMatrix      [hitCounter+1][3] = globalPoint.z()-z0;

    TMatrixD globalError = hit.globalError;
    varianceMatrix[hitCounter][hitCounter]     = globalError[0][0];
    varianceMatrix[hitCounter][hitCounter+1]   = globalError[0][1];
    varianceMatrix[hitCounter+1][hitCounter]   = globalError[1][0];
    varianceMatrix[hitCounter+1][hitCounter+1] = globalError[1][1];
    
    hitCounter+=2;
  }

  //for having the real point varaince matrix varianceMatrix need to be inverted
  try{
    varianceMatrix.Invert();
  }
  catch (cms::Exception &e){
    edm::LogError("RPixPlaneCombinatoryTracking") << "Error in RPixPlaneCombinatoryTracking::fitTrack -> "
    << "Point variance matrix is singular, skipping.";
    CTPPSPixelLocalTrack badTrack;
    badTrack.setValid(false);
    return badTrack;
  }

  TMatrixD zMatrixTranspose(TMatrixD::kTransposed,zMatrix);
  TMatrixD zMatrixTransposeTimesVarianceMatrix(zMatrixTranspose,TMatrixD::kMult,varianceMatrix);
  TMatrixD parametersCovatianceMatrix(zMatrixTransposeTimesVarianceMatrix,TMatrixD::kMult,zMatrix);

    //for having the real parameter covaraince matrix varianceMatrix need to be inverted
  try{
    parametersCovatianceMatrix.Invert();
  }
  catch (cms::Exception &e){
    edm::LogError("RPixPlaneCombinatoryTracking") << "Error in RPixPlaneCombinatoryTracking::fitTrack -> "
    << "Parameter covariance matrix is singular, skipping.";
    CTPPSPixelLocalTrack badTrack;
    badTrack.setValid(false);
    return badTrack;
  }

  TVectorD zMatrixTransposeTimesVarianceMatrixTimesXyCoordinates= zMatrixTransposeTimesVarianceMatrix * xyCoordinates;
  TVectorD parameters = parametersCovatianceMatrix*zMatrixTransposeTimesVarianceMatrixTimesXyCoordinates;

  TVectorD xyCoordinatesMinusZmatrixTimesParameters = xyCoordinates - (zMatrix * parameters);

  // TMatrixD xyCoordinatesMinusZmatrixTimesParametersTranspose(1,2*numberOfPoints);
  //   for(uint32_t c=0; c<2*numberOfPoints; ++c){
  //   xyCoordinatesMinusZmatrixTimesParametersTranspose[0][c] = xyCoordinatesMinusZmatrixTimesParameters[c];
  // }
  
  double_t chiSquare = xyCoordinatesMinusZmatrixTimesParameters * (varianceMatrix * xyCoordinatesMinusZmatrixTimesParameters);
  
  CTPPSPixelLocalTrack goodTrack(z0, parameters, parametersCovatianceMatrix, chiSquare);
  goodTrack.setValid(true);

  for(const auto & hit : pointList){
    CLHEP::Hep3Vector globalPoint = hit.globalPoint;
    TVector3 pointOnDet(parameters[0] + parameters[2]*globalPoint.z(),parameters[1] + parameters[3]*globalPoint.z(),globalPoint.z());
    double xResidual = globalPoint.x() - pointOnDet.X();
    double yResidual = globalPoint.y() - pointOnDet.Y();
    std::pair<double,double> residuals = std::make_pair(xResidual,yResidual);

    TMatrixD globalError = hit.globalError;
    std::pair<double,double> pulls = std::make_pair(xResidual/TMath::Sqrt(globalError[0][0]),yResidual/TMath::Sqrt(globalError[1][1]));

    CTPPSPixelLocalTrack::CTPPSPixelFittedRecHit fittedRecHit(hit.recHit, pointOnDet, residuals, pulls);
    goodTrack.addHit(CTPPSPixelDetId(hit.detId), fittedRecHit);
  }

  return goodTrack;

}

