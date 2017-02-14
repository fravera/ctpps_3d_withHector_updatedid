/****************************************************************************
*
* Author: F.Ferro ferro@ge.infn.it
*
****************************************************************************/

#ifndef CondFormats_CTPPSReadoutObjects_CTPPSPixelAnalysisMask
#define CondFormats_CTPPSReadoutObjects_CTPPSPixelAnalysisMask

#include <set>
#include <map>


//----------------------------------------------------------------------------------------------------

/**
 *\brief Contains data on masked channels of a ROC
 */
class CTPPSPixelROCAnalysisMask
{
  public:
    CTPPSPixelROCAnalysisMask() : fullMask(false) {}

    /// whether all channels of the ROC shall be masked
    bool fullMask;

    /// list of channels to be masked
    std::set<std::pair<unsigned char, unsigned char> > maskedPixels;
};

//----------------------------------------------------------------------------------------------------

/**
 *\brief Channel-mask mapping.
 **/
class CTPPSPixelAnalysisMask
{
  public:
    std::map<uint32_t, CTPPSPixelROCAnalysisMask> analysisMask;

    void insert(const uint32_t &sid, const CTPPSPixelROCAnalysisMask &am);
};

#endif
