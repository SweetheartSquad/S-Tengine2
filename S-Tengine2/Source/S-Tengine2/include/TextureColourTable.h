#pragma once

#include <ProgrammaticTexture.h>

class TextureColourTable : public ProgrammaticTexture{
public:
	TextureColourTable(bool _autoRelease);

	virtual void generateRandomTable();
	
	virtual void loadImageData() override;
};