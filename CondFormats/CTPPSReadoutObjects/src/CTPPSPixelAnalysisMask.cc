/****************************************************************************
*
* F.Ferro ferro@ge.infn.it
*
****************************************************************************/

#include "FWCore/Utilities/interface/typelookup.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"

//----------------------------------------------------------------------------------------------------

void CTPPSPixelAnalysisMask::insert(const uint32_t &sid, const CTPPSPixelROCAnalysisMask &am)
{
  analysisMask[sid] = am;
}

//----------------------------------------------------------------------------------------------------

//TYPELOOKUP_DATA_REG(CTPPSPixelAnalysisMask);
