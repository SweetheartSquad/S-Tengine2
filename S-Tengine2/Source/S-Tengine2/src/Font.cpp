#pragma once

#include <Font.h>
#include <Vox.h>
#include <MeshFactory.h>

Glyph::Glyph(FT_GlyphSlot _glyph) :
	MeshInterface(GL_QUADS, GL_STATIC_DRAW),
	NodeResource(false)
{
	float vx = _glyph->bitmap_left;
	float vy = _glyph->bitmap_top;
	float w = _glyph->bitmap.width;
	float h = _glyph->bitmap.rows;
	advance = _glyph->advance;
	metrics = _glyph->metrics;

	pushVert(Vertex(vx, vy, 0));
	pushVert(Vertex(vx + w, vy, 0));
	pushVert(Vertex(vx + w, vy - h, 0));
	pushVert(Vertex(vx, vy - h, 0));
	setNormal(0, 0.0, 0.0, 1.0);
	setNormal(1, 0.0, 0.0, 1.0);
	setNormal(2, 0.0, 0.0, 1.0);
	setNormal(3, 0.0, 0.0, 1.0);
	setUV(0, 0.0, 0.0);
	setUV(1, 1.0, 0.0);
	setUV(2, 1.0, 1.0);
	setUV(3, 0.0, 1.0);

	scaleModeMag = GL_NEAREST;
	scaleModeMin = GL_NEAREST;
}



GlyphTexture::GlyphTexture(FT_Bitmap _glyph, bool _autoRelease) :
	Texture(true, _autoRelease),
	NodeResource(_autoRelease)
{
	width = _glyph.width;
	height = _glyph.rows;
	data = new unsigned char[_glyph.rows*_glyph.width];
	memcpy(data, _glyph.buffer, _glyph.rows*_glyph.width);
	//data = _glyph.buffer;
	channels = 2;
}
GlyphTexture::~GlyphTexture(){
	//data = nullptr; // freetype will free this data (I think? It's just a reference to face->glyph->bitmap anyway)
}

void GlyphTexture::load(){
	if(!loaded){
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureId);
		checkForGlError(0,__FILE__,__LINE__);
		glBindTexture(GL_TEXTURE_2D, textureId);
		checkForGlError(0,__FILE__,__LINE__);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	NodeLoadable::load();
}




Font::Font(std::string _fontSrc, int _size, bool _autoRelease) :
	NodeResource(_autoRelease),
	face(nullptr)
{
	if(FT_New_Face(vox::freeTypeLibrary, _fontSrc.c_str(), 0, &face) != 0) {
		std::cerr << "Couldn't load font: " << _fontSrc;
	}
	size = _size;
	lineGapRatio = 1.2f;
}

Font::~Font(){
	for(auto c : meshes){
		delete c.second;
	}
	for(auto c : textures){
		delete c.second;
	}
	FT_Done_Face(face);
}

void Font::load(){
	for(auto c : meshes){
		c.second->load();
	}
	for(auto c : textures){
		c.second->load();
	}
}

void Font::unload(){
	for(auto c : textures){
		c.second->unload();
	}
	for(auto c : meshes){
		c.second->unload();
	}
}

GlyphTexture * Font::getTextureForChar(wchar_t _char){
	loadGlyph(_char);
	auto t = textures.find(_char);
	GlyphTexture * res;
	if(t == textures.end()){
		FT_Set_Pixel_Sizes(face, 0, size);
		FT_Load_Char(face, _char, FT_LOAD_RENDER);
		GlyphTexture * tex = new GlyphTexture(face->glyph->bitmap, false);
		textures.insert(std::pair<wchar_t, GlyphTexture *>(_char, tex));
		res = tex;
		res->load();
	}else{
		res = t->second;
	}
	return res;
}

Glyph* Font::getMeshInterfaceForChar(wchar_t _char){
	auto t = meshes.find(_char);
	Glyph * res;
	if(t == meshes.end()){
		loadGlyph(_char);
		Glyph * mesh = new Glyph(face->glyph);
		mesh->autoRelease = false;
		mesh->pushTexture2D(getTextureForChar(_char));
		meshes.insert(std::pair<char, Glyph *>(_char, mesh));
		res = mesh;
		res->load();
	}else{
		res = t->second;
	}
	return res;
}

glm::vec2 Font::getGlyphWidthHeight(wchar_t _char){
	loadGlyph(_char);
	return glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
}

glm::vec2 Font::getGlyphXY(wchar_t _char){
	loadGlyph(_char);
	return glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
}

void Font::loadGlyph(wchar_t _char){
	FT_Set_Pixel_Sizes(face, 0, size);
	FT_Load_Char(face, _char, FT_LOAD_RENDER);
}

float Font::getLineHeight(){
	return lineGapRatio * size;//((face->size->metrics.ascender + face->size->metrics.descender)/64);
}