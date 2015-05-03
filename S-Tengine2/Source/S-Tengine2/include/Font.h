#pragma once

#include <string>
#include <map>

#include <Texture.h>
#include <node/NodeResource.h>
#include "MeshInterface.h"

class GlyphTexture : public Texture{
public:
	GlyphTexture(FT_Bitmap _glyph, bool _autoRelease);
	~GlyphTexture();
	virtual void load() override;
};

class Font : public NodeResource{
public:	
	
	FT_Face face;
	std::map<char, GlyphTexture *> textures;
	std::map<char, MeshInterface *> meshes;

	explicit Font(std::string _fontSrc, int size, bool _autoRelease);
	~Font();
	
	void load() override;
	void unload() override;

	GlyphTexture * getTextureForChar(char _char);
	MeshInterface * getMeshInterfaceForChar(char _char);
	glm::vec2 getGlyphWidthHeight(char _char);
	glm::vec2 getGlyphXY(char _char);
	void loadGlyph(char _char);

private:
	int size;
};