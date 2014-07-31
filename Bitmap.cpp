#include "Bitmap.h"


Bitmap::Bitmap(const char* filename)
{
    if (filename)
    {
        if (!LoadBitmap(filename))
        {
            std::cout << "비트맵 로딩 실패" << std::endl;
        }
    }
}


Bitmap::~Bitmap()
{
    if (data)
    {
        delete[] data;
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
    pitch = (width * bytesPerPixel + 3) & ~3;
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

    /*const int bytes = width * height * bytesPerPixel;
    data.resize(bytes);
    unsigned char* const destBegin = static_cast<unsigned char*>(&data[0]);

    const int destPitch = width * bytesPerPixel;
    const int gap = pitch - destPitch;
    unsigned char* curDest = destBegin + height * destPitch;
    for (int y = height; y > 0; --y)
    {
        curDest -= destPitch;
        fread(curDest, destPitch, 1, fp);
        if (gap > 0)
        {
            fseek(fp, gap, SEEK_CUR);
        }
        
        for (int x = 0; x < width; ++x)
        {
            char temp = curDest[x * 3];
            curDest[x * 3] = curDest[x * 3 + 2];
            curDest[x * 3 + 2] = temp;
        }
    }*/
    
    // 이미지 데이터를 그대로 읽어오고 GetPixel() 호출시 정확한 위치를 계산하여 리턴하는 방식임
    data = new unsigned char[infoHeader.biSizeImage];
    fread(data, sizeof(unsigned char), infoHeader.biSizeImage, fp);
    
    const int expectedEnd =
        fileHeader.bfOffBits + infoHeader.biSizeImage;
    const int dataEndPos = ftell(fp);
    return dataEndPos == expectedEnd;
}

RGBQurd Bitmap::GetPixel(int x, int y) const
{
    //int idx = x + infoHeader.biWidth * y;
    int idx = x + pitch * (infoHeader.biHeight - y - 1);
    int palleteIdx = static_cast<int>(data[idx]);
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