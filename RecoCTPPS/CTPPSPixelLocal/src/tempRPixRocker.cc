#include <iostream>


#include "RecoCTPPS/CTPPSPixelLocal/interface/tempRPixRocker.h"



void tempRPixRocker::run()
{
// checking that the mapping is read
  printf("* DAQ mapping\n");
  for (const auto &p : mapping_){
    std::cout << "    " << p.first << " -> " << p.second << std::endl;
    
  }



// here let's assume that we have read a word from which we get fed, ilink, roc, dcol, pxl, adc






  return;
}
