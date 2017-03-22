#ifndef RPixErrorChecker_H
#define RPixErrorChecker_H
/** \class RPixErrorChecker
 *
 *  
 */

//#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "FWCore/Utilities/interface/typedefs.h"

#include <vector>
#include <map>

class FEDRawData;

//class SiPixelFrameConverter;

class RPixErrorChecker {

public:
//  typedef unsigned int Word32;
//  typedef long long Word64;
  typedef cms_uint32_t Word32;
  typedef cms_uint64_t Word64;

//  typedef std::vector<SiPixelRawDataError> DetErrors;
//  typedef std::map<cms_uint32_t, DetErrors> Errors;

  RPixErrorChecker();

  // void setErrorStatus(bool ErrorStatus);

  bool checkCRC(bool& errorsInEvent, int fedId, const Word64* trailer);

  bool checkHeader(bool& errorsInEvent, int fedId, const Word64* header);

  bool checkTrailer(bool& errorsInEvent, int fedId, int nWords, const Word64* trailer);

  //bool checkROC(bool& errorsInEvent, int fedId, const SiPixelFrameConverter* converter, Word32& errorWord, Errors& errors);
bool checkROC(bool& errorsInEvent, int fedId, Word32& errorWord);



  // void conversionError(int fedId, const SiPixelFrameConverter* converter,      int status, Word32& errorWord, Errors& errors);

private:

//  bool includeErrors;

//  cms_uint32_t errorDetId(const SiPixelFrameConverter* converter,     int errorType, const Word32 & word) const;

};

#endif
