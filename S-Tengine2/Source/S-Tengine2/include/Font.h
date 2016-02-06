#pragma once

#include <string>
#include <map>

#include <Texture.h>
#include <node/NodeResource.h>
#include <MeshInterface.h>

class Glyph : public MeshInterface{
public:
	Glyph(FT_GlyphSlot _glyph, wchar_t _character, bool _antiAliased);
	FT_Vector advance;
	FT_Glyph_Metrics metrics;
	wchar_t character;
};

class GlyphTexture : public Texture{
public:
	GlyphTexture(FT_Bitmap _glyph, bool _autoRelease);
	~GlyphTexture();
	virtual void load() override;
};

class Font : public NodeResource{
public:	
	
	FT_Face face;
	float lineGapRatio;
	std::map<wchar_t, GlyphTexture *> textures;
	std::map<wchar_t, Glyph *> meshes;

	// if true, font meshes use the standard scale mode
	// if false, font meshes use GL_NEAREST
	bool antiAliased;

	explicit Font(std::string _fontSrc, int size, bool _autoRelease);
	~Font();
	
	void load() override;
	void unload() override;

	GlyphTexture * getTextureForChar(wchar_t _char);
	Glyph * getMeshInterfaceForChar(wchar_t _char);
	glm::vec2 getGlyphWidthHeight(wchar_t _char);
	glm::vec2 getGlyphXY(wchar_t _char);
	void loadGlyph(wchar_t _char);
	float getLineHeight();

private:
	int size;
};