#include "CEGUIExceptions.h"
#include "CEGUISDLImageCodec.h"
#include "CEGUILogger.h"
#include "CEGUISize.h"

#include <FreeImage.h>
namespace
{
    void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
    {
        CEGUI::Logger::getSingleton().logEvent(
            CEGUI::String("FreeImage error (") + FreeImage_GetFormatFromFIF(fif) + "): " + message, CEGUI::Errors);
    }

}

// Start of CEGUI namespace section
namespace CEGUI
{
SDLImageCodec::SDLImageCodec()
    : ImageCodec("FreeImageCodec - FreeImage based image codec")
{
    FreeImage_Initialise(true);
    FreeImage_SetOutputMessage(&FreeImageErrorHandler);

    // Getting extensions
    for (int i = 0; i < FreeImage_GetFIFCount(); ++i)
    {
        String exts(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i));

        // Replace commas with spaces
        for (size_t i = 0; i < exts.length(); ++i)
            if (exts[i] == ',')
                exts[i] = ' ';

        // Add space after existing extensions
        if (!d_supportedFormat.empty())
            d_supportedFormat += ' ';

        d_supportedFormat += exts;
    }
}

SDLImageCodec::~SDLImageCodec()
{
    FreeImage_DeInitialise();
}

Texture* SDLImageCodec::load(const RawDataContainer& data, Texture* result)
{
    int len = (int)data.getSize();
    FIMEMORY *mem = 0;
    FIBITMAP *img = 0;
    Texture *retval = 0;

    CEGUI_TRY
    {
        mem = FreeImage_OpenMemory((BYTE*)data.getDataPtr(), len);
        if (mem == 0)
            CEGUI_THROW(MemoryException("Unable to open memory stream, FreeImage_OpenMemory failed"));

        FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(mem, len);

        if (fif == FIF_UNKNOWN) // it may be that it's TARGA or MNG
        {
            fif = FIF_TARGA;
            img = FreeImage_LoadFromMemory(fif, mem, 0);

            if (img == 0)
            {
                fif = FIF_MNG;
                img = FreeImage_LoadFromMemory(fif, mem, 0);
            }
        }
        else
            img = FreeImage_LoadFromMemory(fif, mem, 0);

        if (img == 0)
            CEGUI_THROW(GenericException("Unable to load image, FreeImage_LoadFromMemory failed"));

        FIBITMAP *newImg = FreeImage_ConvertTo32Bits(img);
        if (newImg == 0)
            CEGUI_THROW(GenericException("Unable to convert image, FreeImage_ConvertTo32Bits failed"));
        FreeImage_Unload(img);
        img = newImg;
        newImg = 0;

        // FreeImage pixel format for little-endian architecture (which CEGUI
        // supports) is like BGRA. We need to convert that to RGBA.
        //
        // It is now:
        // RED_MASK		0x00FF0000
        // GREEN_MASK	0x0000FF00
        // BLUE_MASK	0x000000FF
        // ALPHA_MASK	0xFF000000
        //
        // It should be:
        // RED_MASK		0x000000FF
        // GREEN_MASK	0x0000FF00
        // BLUE_MASK	0x00FF0000
        // ALPHA_MASK	0xFF000000

        uint pitch = FreeImage_GetPitch(img);
        uint height = FreeImage_GetHeight(img);
        uint width = FreeImage_GetWidth(img);
        uint8 *rawBuf = new uint8[width * height << 2];

        // convert the bitmap to raw bits (top-left pixel first) 
        FreeImage_ConvertToRawBits(rawBuf, img, pitch, 32,
            FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);

        // We need to convert pixel format a little
        // NB: little endian only - I think(!)
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
        for (uint i = 0; i < height; ++i)
        {
            for (uint j = 0; j < width; ++j)
            {
                uint p = *(((uint*)(rawBuf + i * pitch)) + j);
                uint r = (p >> 16) & 0x000000FF;
                uint b = (p << 16) & 0x00FF0000;
                p &= 0xFF00FF00;
                p |= r | b;
                // write the adjusted pixel back
                *(((uint*)(rawBuf + i * pitch)) + j) = p;
            }
        }
#endif
        FreeImage_Unload(img);
        img = 0;

        result->loadFromMemory(rawBuf, Size(width, height), Texture::PF_RGBA);
        delete [] rawBuf;
        retval = result;
    }
    CEGUI_CATCH(Exception&)
    {
    }

    if (img != 0) FreeImage_Unload(img);
    if (mem != 0) FreeImage_CloseMemory(mem);

    return retval;
}


} // End of CEGUI namespace section

