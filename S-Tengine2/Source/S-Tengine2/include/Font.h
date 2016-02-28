#pragma once

#include <string>
#include <map>

#include <Texture.h>
#include <node/NodeResource.h>
#include <MeshInterface.h>

class GlyphMesh : public QuadMesh{
public:
	GlyphMesh(FT_GlyphSlot _glyph, bool _antiAliased);
};

class GlyphTexture : public Texture{
public:
	GlyphTexture(FT_Bitmap _glyph);
	virtual void load() override;
};

class Glyph : public NodeResource{
public:
	GlyphMesh * mesh;
	GlyphTexture * texture;
	
	FT_Int bitmap_top, bitmap_left;
	FT_Vector advance;
	FT_Glyph_Metrics metrics;
	wchar_t character;

	Glyph(FT_GlyphSlot _glyph, wchar_t _character, bool _antiAliased);
	~Glyph();
	
	virtual void load() override;
	virtual void unload() override;
};

class Font : public NodeResource{
public:	
	FT_Face face;
	float lineGapRatio;

	std::map<wchar_t, Glyph *> glyphs;

	// if true, font meshes use the standard scale mode
	// if false, font meshes use GL_NEAREST
	bool antiAliased;

	explicit Font(std::string _fontSrc, int size, bool _autoRelease);
	~Font();
	
	void load() override;
	void unload() override;

	Glyph * getGlyphForChar(wchar_t _character);
	GlyphTexture * getTextureForChar(wchar_t _character);
	GlyphMesh * getMeshInterfaceForChar(wchar_t _character);
	glm::vec2 getGlyphWidthHeight(wchar_t _character);
	glm::vec2 getGlyphXY(wchar_t _character);
	void loadGlyph(wchar_t _character);
	float getLineHeight();

private:
	int size;
};