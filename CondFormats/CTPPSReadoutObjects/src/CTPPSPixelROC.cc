#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelROC.h"

#include "DataFormats/DetId/interface/DetId.h"

#include <sstream>
#include <algorithm>
using namespace std;


// Constructor with transformation frame initilization - NEVER CALLED
CTPPSPixelROC::CTPPSPixelROC(uint32_t du, int idDU, int idLk)
  : theDetUnit(du), theIdDU(idDU), theIdLk(idLk) {
  theIndices = new CTPPSPixelIndices();
}

/*

string PixelROC::print(int depth) const {

  ostringstream out;
  bool barrel = PixelModuleName::isBarrel(theDetUnit);
  DetId detId(theDetUnit);
  if (depth-- >=0 ) {
    out <<"======== PixelROC ";
    //out <<" unit: ";
    //if (barrel) out << PixelBarrelName(detId).name();
    //else        out << PixelEndcapName(detId).name(); 
    if (barrel) out << " barrel ";
    else        out << " endcap "; 
    out <<" ("<<theDetUnit<<")"
        <<" idInDU: "<<theIdDU
        <<" idInLk: "<<theIdLk
        <<endl;
  }
  return out.str();
}
*/
