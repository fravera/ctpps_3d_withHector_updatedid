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
 * | 13 bits  |  2 bits |   10 bits |  1  bit          |       4 bits                        |  2bits     |
 * |  empty  | empty  |  FED ID  |    FMC   0-1  |   fiber index      0-12          | ROC 0-2 |
 * \endverbatim
 *
 **/
class CTPPSPixelFramePosition
{
  public:
    static const unsigned int offsetROC = 0, maskROC = 0x3;
    static const unsigned int offsetChannelIdx = 2, maskChannelIdx = 0xF;
    static const unsigned int offsetFMCId = 6, maskFMCId = 0x1;
    static const unsigned int offsetFEDId = 7, maskFEDId = 0x3FF;
  

    /// the preferred constructor
    CTPPSPixelFramePosition( unsigned short FEDId, unsigned short FMCId, unsigned short ChannelIdx, unsigned short ROC) :
      rawPosition(ROC<<offsetROC | ChannelIdx<<offsetChannelIdx | FMCId<<FMCId | FEDId<<offsetFEDId )
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

    unsigned short getChannelIdx() const       { return (rawPosition >> offsetChannelIdx) & maskChannelIdx; }

    void setChannelIdx(unsigned short v)
    { v &= maskChannelIdx; rawPosition &= 0xFFFFFFFF - (maskChannelIdx << offsetChannelIdx); rawPosition |= (v << offsetChannelIdx); }

    unsigned short getROC() const  { return (rawPosition >> offsetROC) & maskROC; }
    
    void setROC(unsigned short v)
    { v &= maskROC; rawPosition &= 0xFFFFFFFF - (maskROC << offsetROC); rawPosition |= (v << offsetROC); }

   unsigned short getFMCId() const    { return (rawPosition >> offsetFMCId) & maskFMCId; }

    void setFMCId(unsigned short v)
    { v &= maskFMCId; rawPosition &= 0xFFFFFFFF - (maskFMCId << offsetFMCId); rawPosition |= (v << offsetFMCId); }


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
    /// prints 5-digit hex number,
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
