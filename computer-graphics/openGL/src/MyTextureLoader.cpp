#include "MyTextureLoader.h"

MyTextureLoader::MyTextureLoader()
{
    bits    = NULL;
    pixels  = NULL;
    dib     = NULL;
}

bool MyTextureLoader::load(const char* textureSrc)
{
    dib = FreeImage_Load(FIF_PNG, textureSrc, PNG_DEFAULT);

    if (dib)
    {
        bits = new BYTE[unsigned __int64(FreeImage_GetWidth(dib)) * unsigned __int64(FreeImage_GetHeight(dib) * 4)];
        pixels = (BYTE*)FreeImage_GetBits(dib);

        for (int pix = 0; pix < FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
        {
            bits[pix * 4 + 0] = pixels[pix * 4 + 2];
            bits[pix * 4 + 1] = pixels[pix * 4 + 1];
            bits[pix * 4 + 2] = pixels[pix * 4 + 0];
            bits[pix * 4 + 3] = pixels[pix * 4 + 3];
        }

        return true;
    }
    else return false;
}

BYTE* MyTextureLoader::getBits()
{
    return bits;
}

int MyTextureLoader::getH()
{
    return FreeImage_GetHeight(dib);
}

int MyTextureLoader::getW()
{
   return FreeImage_GetWidth(dib);
}

void MyTextureLoader::del()
{
    FreeImage_Unload(dib);
    delete bits;
    delete pixels;
}
