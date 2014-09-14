#pragma once

#include <SOIL.h>

class Resource
{
private:
	Resource();
public:
	~Resource();

	/* Readable Image Formats:
	* BMP - non-1bpp, non-RLE (from stb_image documentation)
	* PNG - non-interlaced (from stb_image documentation)
	* JPG - JPEG baseline (from stb_image documentation)
	* TGA - greyscale or RGB or RGBA or indexed, uncompressed or RLE
	* DDS - DXT1/2/3/4/5, uncompressed, cubemaps (can't read 3D DDS files yet)
	* PSD - (from stb_image documentation)
	* HDR - converted to LDR, unless loaded with *HDR* functions (RGBE or RGBdivA or RGBdivA2)*/
	static unsigned char* loadImage(const char* _src,  int _width, int _height, int _SOILLoadMode);

	//Free image data memory
	static void freeImageData(unsigned char* _image);
};