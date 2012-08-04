#include "CEGUISDLImageCodecModule.h" 
#include "CEGUISDLImageCodec.h"


CEGUI::ImageCodec* createImageCodec(void)
{
  return new CEGUI::SDLImageCodec();
}


void destroyImageCodec(CEGUI::ImageCodec* imageCodec)
{
  delete imageCodec;
  
}
