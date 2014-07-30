#pragma once
#include <iostream>
#include "defStruct.h"

#pragma pack(push, 1)

struct BitmapFileHeader
{
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
};

struct BitmapInfoHeader
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
};

struct RGBQurd
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;

    inline bool operator==(RGBQurd arg) const
    {
        if (rgbBlue == arg.rgbBlue && rgbGreen == arg.rgbGreen && rgbRed == arg.rgbRed)
            return true;
        else
            return false;
    }

    inline bool operator!=(RGBQurd arg) const
    {
        return !operator==(arg);
    }
};

#pragma pack(pop)

struct ScopedFilePointer
{
    ScopedFilePointer(const char* filename, const char* mode) : fp_(NULL)
    {
        fp_ = fopen(filename, mode);
    }
    ~ScopedFilePointer()
    {
        if (fp_)
        {
            fclose(fp_);
        }
    }
    operator FILE* () const { return fp_; }

private:
    FILE* fp_;
    void operator==(const ScopedFilePointer&);
    ScopedFilePointer(const ScopedFilePointer&);
};

const RGBQurd Black{ 0, 0, 0, 0 };
//const RGBQurd White{ 255, 255, 255, 0 };

class Bitmap
{
private:
    const static int PalleteSize = 256;

    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    RGBQurd pallete[PalleteSize];
    unsigned char* bmpPixel;

public:
    explicit Bitmap(const char* filename);
    ~Bitmap();

    bool LoadBitmap(const char* filename);
    RGBQurd GetPixel(int x, int y) const;
    RGBQurd GetPixel(Point p) const;
    int GetWidth() const;
    int GetHeight() const;
};

