/****************************************************************************
*
* 
* Authors: 
*  F.Ferro ferro@ge.infn.it
*
****************************************************************************/

#ifndef CondFormats_CTPPSReadoutObjects_CTPPSPixelFramePosition
#define CondFormats_CTPPSReadoutObjects_CTPPSPixelFramePosition

#include <iostream>
#include <string>

/**
 * Uniquely identifies the DAQ channel through which a ROC frame has been received.
 * 
 * The internal representation has the following structure:
 * \verbatim
 * |                                     32 bits raw position                                                                      |
 * | 13 bits  |  2 bits |   10 bits |  1  bit          |       3 bits                        |  3bits     |
 * |  empty  | empty  |  FED ID  |    FMC   0-1  |   fiber index      0-5          | ROC 0-5 |
 * \endverbatim
 *
 **/
class CTPPSPixelFramePosition
{
  public:
    static const unsigned int offsetROC = 0, maskROC = 0x7;
    static const unsigned int offsetFiberIdx = 3, maskFiberIdx = 0x7;
    static const unsigned int offsetFMCId = 6, maskFMCId = 0x1;
    static const unsigned int offsetFEDId = 7, maskFEDId = 0x3FF;
  

    /// the preferred constructor
    CTPPSPixelFramePosition( unsigned short FEDId, unsigned short FMCId, unsigned short FiberIdx, unsigned short ROC) :
      rawPosition(ROC<<offsetROC | FiberIdx<<offsetFiberIdx | FMCId<<FMCId | FEDId<<offsetFEDId )
    {
    }

    /// don't use this constructor unless you have a good reason
    CTPPSPixelFramePosition(unsigned int pos = 0) : rawPosition(pos)
    {
    }

    ~CTPPSPixelFramePosition()
    {
    }

    /// recomended getters and setters

    unsigned short getFEDId() const
    {
      return (rawPosition >> offsetFEDId) & maskFEDId;
    }

    void setFEDId(unsigned short v)
    { v &= maskFEDId; rawPosition &= 0xFFFFFFFF - (maskFEDId << offsetFEDId); rawPosition |= (v << offsetFEDId); }

    unsigned short getFiberIdx() const       { return (rawPosition >> offsetFiberIdx) & maskFiberIdx; }

    void setFiberIdx(unsigned short v)
    { v &= maskFiberIdx; rawPosition &= 0xFFFFFFFF - (maskFiberIdx << offsetFiberIdx); rawPosition |= (v << offsetFiberIdx); }

    unsigned short getROC() const  { return (rawPosition >> offsetROC) & maskROC; }
    
    void setROC(unsigned short v)
    { v &= maskROC; rawPosition &= 0xFFFFFFFF - (maskROC << offsetROC); rawPosition |= (v << offsetROC); }

   unsigned short getFMCId() const    { return (rawPosition >> offsetFMCId) & maskFMCId; }

    void setFMCId(unsigned short v)
    { v &= maskFMCId; rawPosition &= 0xFFFFFFFF - (maskFMCId << offsetFMCId); rawPosition |= (v << offsetFMCId); }


 
    /// don't use this method unless you have a good reason
    unsigned int getRawPosition() const
    {
      return rawPosition;
    }

    bool operator < (const CTPPSPixelFramePosition &pos) const
    {
      return (rawPosition < pos.rawPosition);
    }

    bool operator == (const CTPPSPixelFramePosition &pos) const
    {
      return (rawPosition == pos.rawPosition);
    }

    /// Condensed representation of the DAQ channel.
    /// prints 5-digit hex number, the digits correspond to SubSystem, TOTFED ID, FMC ID, 
    /// GOH ID, index within fiber in this order
    friend std::ostream& operator << (std::ostream& s, const CTPPSPixelFramePosition &fp);
    
    /// prints XML formatted DAQ channel to stdout
    void printXML();

    /// Sets attribute with XML name 'attribute' and value 'value'.
    /// Also turns on attribute presents bit in the flag parameter
    /// returns 0 if the attribute is known, non-zero value else
    unsigned char setXMLAttribute(const std::string &attribute, const std::string &value, unsigned char &flag);

    /// returns true if all attributes have been set
    static bool checkXMLAttributeFlag(unsigned char flag)
    {
      return (flag == 0xF);
    }

  protected:
    unsigned int rawPosition;
};

#endif
