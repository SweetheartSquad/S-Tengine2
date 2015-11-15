#pragma once

#include <TextureColourTable.h>
#include <NumberUtils.h>

TextureColourTable::TextureColourTable(bool _autoRelease) :
	Texture("", true, _autoRelease),
	NodeResource(_autoRelease)
{
	channels = 4;
	width = 16;
	height = 16;
	numPixels = width * height;
	numBytes = numPixels * channels;
};

void TextureColourTable::generateRandomTable(){
	// set the first and last pixels to be transparent
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[numBytes-1] = 0;
	data[numBytes-2] = 0;
	data[numBytes-3] = 0;
	data[numBytes-4] = 0;

	// set the colour on all of the other pixels
	for(unsigned long int i = 4; i < numBytes-4; i += 4){
		data[i+0] = sweet::NumberUtils::randomInt(0, 255);
		data[i+1] = sweet::NumberUtils::randomInt(0, 255);
		data[i+2] = sweet::NumberUtils::randomInt(0, 255);
		data[i+3] = 255;
	}
}

void TextureColourTable::loadImageData(){
	data = (unsigned char *)malloc(sizeof(unsigned char) * numBytes);
	
	generateRandomTable();
}