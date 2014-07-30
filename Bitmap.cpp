#include "Bitmap.h"


Bitmap::Bitmap(const char* filename)
{
    if (filename)
    {
        LoadBitmap(filename);
    }
}


Bitmap::~Bitmap()
{
    if (bmpPixel)
    {
        delete[] bmpPixel;
    }
}

bool Bitmap::LoadBitmap(const char* filename)
{
    if (!filename || filename[0] == '\0')
    {
        fprintf(stderr, "filename required!\n", filename);
        return false;
    }
    const ScopedFilePointer fp(filename, "rb");

    if (!fp)
    {
        fprintf(stderr, "Could not open file \"%s\"!\n", filename);
        return false;
    }

    fileHeader = {};
    fread(&fileHeader, sizeof(fileHeader), 1, fp);
    if (fileHeader.bfType != ('B' | ((int)'M' << 8)))
    {
        fprintf(stderr, "\"%s\" is not a bitmap file!\n", filename);
        return false;
    }

    infoHeader = {};
    fread(&infoHeader, sizeof(infoHeader), 1, fp);

    const int width = infoHeader.biWidth;
    const int height = infoHeader.biHeight;
    const int bitsPerPixel = infoHeader.biBitCount;
    const int bytesPerPixel = bitsPerPixel / 8;
    const int pitch = (width * bytesPerPixel + 3) & ~3;
    const int dataSize = pitch * height;

    if (width <= 0 || height <= 0 ||
        infoHeader.biPlanes != 1 ||
        bitsPerPixel != 8 ||
        infoHeader.biCompression != 0 ||
        static_cast<int>(infoHeader.biSizeImage) != dataSize
        )
    {
        fprintf(stderr, "\"%s\" is unsupported format!\n", filename);
        fprintf(stderr,
            "  Width:%d, Height:%d, Bpp:%d, Plane:%d, "
            "Compression:%d, Size:%d\n",
            width, height, infoHeader.biBitCount, infoHeader.biPlanes,
            infoHeader.biCompression, infoHeader.biSizeImage);
        return false;
    }

    fread(pallete, sizeof(RGBQurd), 256, fp);

    const int dataBeginPos = ftell(fp);

    if (dataBeginPos != static_cast<int>(fileHeader.bfOffBits))
    {
        fprintf(stderr, "\"%s\" is unsupported format!\n", filename);
        fprintf(stderr, "  OffsetBits:%d\n", fileHeader.bfOffBits);
        return false;
    }

    bmpPixel = new unsigned char[infoHeader.biSizeImage];
    fread(bmpPixel, sizeof(unsigned char), infoHeader.biSizeImage, fp);
    fclose(fp);

    return true;
}

RGBQurd Bitmap::GetPixel(int x, int y) const
{
    int idx = x + infoHeader.biWidth * (infoHeader.biHeight - y - 1);
    int palleteIdx = static_cast<int>(bmpPixel[idx]);
    return pallete[palleteIdx];
}

RGBQurd Bitmap::GetPixel(Point p) const
{
    return GetPixel(p.x, p.y);
}

int Bitmap::GetWidth() const
{
    return infoHeader.biWidth;
}

int Bitmap::GetHeight() const
{
    return infoHeader.biHeight;
}