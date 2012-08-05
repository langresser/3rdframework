#include "CEGUIExceptions.h"
#include "CEGUISDLImageCodec.h"
#include "CEGUILogger.h"
#include "CEGUISize.h"

#include "SDL_image.h"

// Start of CEGUI namespace section
namespace CEGUI
{
SDLImageCodec::SDLImageCodec()
    : ImageCodec("FreeImageCodec - FreeImage based image codec")
{
//    IMG_Init();
}

SDLImageCodec::~SDLImageCodec()
{
 //   IMG_Quit();
}

Texture* SDLImageCodec::load(const char* fileName, Texture* result)
{
	SDL_Surface* surface = IMG_Load(fileName);
    Texture *retval = 0;
	SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
	int width = rgbaSurface->w;
	int height = rgbaSurface->h;
	result->loadFromMemory(rgbaSurface->pixels, Size(width, height), Texture::PF_RGBA);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(rgbaSurface);
	return result;
}

Texture* SDLImageCodec::load(const RawDataContainer& data, Texture* result)
{
	return NULL;
}


} // End of CEGUI namespace section

