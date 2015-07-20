#pragma once

#include <Texture.h>

class TextureColourTable : public Texture{
public:
	TextureColourTable(bool _autoRelease);
	~TextureColourTable();

	void generateRandomTable();

	virtual void loadImageData() override;
};