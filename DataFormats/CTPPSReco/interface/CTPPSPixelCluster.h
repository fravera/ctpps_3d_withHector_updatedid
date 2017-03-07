#ifndef DataFormats_CTPPS_CTPPSPixelCluster_h
#define DataFormats_CTPPS_CTPPSPixelCluster_h

/*
  \class CTPPSPixelCluster 
  \brief CTPPSPixelCluster stores the information of CTPPS Tracker clusters of 3D pixels
// Author: F.Ferro - INFN Genova - 2016
*/


#include <vector>
#include <cstdint>
#include <cassert>


class CTPPSPixelCluster {

public:

  CTPPSPixelCluster() {}
  static const unsigned int MAXSPAN=127;   // ??
  static const unsigned int MAXPOS=2047;  // ??


CTPPSPixelCluster(unsigned int isize, uint16_t * adcs,         
	    uint16_t const * rowpos,  uint16_t const * colpos, 
	    uint16_t const  rowmin,  uint16_t const  colmin) :
  thePixelOffset(2*isize), // the pixel offset is the pixel position inside the cluster wrt rowmin (even positions) and colmin (odd positions)
    thePixelADC(adcs, adcs+isize)   
    {

      uint16_t maxCol = 0;
      uint16_t maxRow = 0;
      for (unsigned int i=0; i!=isize; ++i) {
	uint16_t rowoffset = rowpos[i]-rowmin;
	uint16_t coloffset = colpos[i]-colmin;
	thePixelOffset[i*2] = std::min(uint16_t(MAXSPAN),rowoffset);
	thePixelOffset[i*2+1] = std::min(uint16_t(MAXSPAN),coloffset);
	if (rowoffset > maxRow) maxRow = rowoffset; 
	if (coloffset > maxCol) maxCol = coloffset; 
      }
      
      
      theMinPixelRow = rowmin;
      thePixelRowSpan = std::min(maxRow, uint16_t(MAXSPAN));
                
      theMinPixelCol = colmin;
      thePixelColSpan = std::min(maxCol, uint16_t(MAXSPAN));
    }

// barycenter

  float avg_row() const {
    float qm = 0.0;
    int isize = thePixelADC.size();
    for (int i=0; i<isize; ++i)
      qm += float(thePixelADC[i]) * (thePixelOffset[i*2] + minPixelRow() + 0.5f);
    return qm/charge();
  }
   
  float avg_col() const {
    float qm = 0.0;
    int isize = thePixelADC.size();
    for (int i=0; i<isize; ++i)
      qm += float(thePixelADC[i]) * (thePixelOffset[i*2+1]  + minPixelCol() + 0.5f);
    return qm/charge();
  }

//cluster charge

  inline float charge() const {
    float qm = 0.0;
    int isize = thePixelADC.size();
    for (int i=0; i<isize; ++i) 
      qm += float(thePixelADC[i]);
    return qm;
  }

  // Return number of pixels.
  int size() const { return thePixelADC.size();}
   
  // Return cluster dimension in the x direction.
  int sizeRow() const { return rowSpan() +1;}
   
  // Return cluster dimension in the y direction.
  int sizeCol() const { return colSpan() +1;}

  inline int minPixelRow() const { return theMinPixelRow;}
  inline int minPixelCol() const { return theMinPixelCol;}

  
  int colSpan() const {return thePixelColSpan; }
  int rowSpan() const { return thePixelRowSpan; }


  const std::vector<uint8_t> & pixelOffset() const { return thePixelOffset;}
  const std::vector<uint16_t> & pixelADC() const { return thePixelADC;}

  uint16_t pixelRow(int i) const {
    return minPixelRow() + thePixelOffset[i*2];
  }
  uint16_t pixelCol(int i) const {
    return minPixelCol() + thePixelOffset[i*2+1];
  }
  uint16_t pixelADC(int i) const {
    return thePixelADC[i];
  }
  
private:

  std::vector<uint8_t>  thePixelOffset;
  std::vector<uint16_t> thePixelADC;

  
  
  uint16_t theMinPixelRow=MAXPOS; // Minimum pixel index in the row direction (low edge).
  uint16_t theMinPixelCol=MAXPOS; // Minimum pixel index in the col direction (left edge).
  uint8_t thePixelRowSpan=0; // Span pixel index in the row direction (low edge).
  uint8_t thePixelColSpan=0; // Span pixel index in the col direction (left edge).
   
 

};

inline bool operator<( const CTPPSPixelCluster& one, const CTPPSPixelCluster& two) {
  if ( one.minPixelRow() < two.minPixelRow() ) {
    return true;
  } else if ( one.minPixelRow() > two.minPixelRow() ) {
    return false;
  } else if ( one.minPixelCol() < two.minPixelCol() ) {
    return true;
  } else {
    return false;
  }
}

#endif
