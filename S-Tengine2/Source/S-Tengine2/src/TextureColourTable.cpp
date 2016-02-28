#pragma once

#include <TextureColourTable.h>
#include <NumberUtils.h>

TextureColourTable::TextureColourTable(bool _autoRelease) :
	ProgrammaticTexture(nullptr, _autoRelease),
	NodeResource(_autoRelease)
{
	allocate(256, 1, 4);
};

void TextureColourTable::generateRandomTable(){
	// set the colour on all of the pixels
	for(unsigned long int i = 0; i < numBytes; i += 4){
		data[i+0] = sweet::NumberUtils::randomInt(0, 255);
		data[i+1] = sweet::NumberUtils::randomInt(0, 255);
		data[i+2] = sweet::NumberUtils::randomInt(0, 255);
		data[i+3] = 255;
	}
}

void TextureColourTable::loadImageData(){
	generateRandomTable();
}