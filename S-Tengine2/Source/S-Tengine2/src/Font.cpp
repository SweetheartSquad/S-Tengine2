#pragma once

#include <Font.h>
#include <Sweet.h>
#include <MeshFactory.h>

GlyphMesh::GlyphMesh(FT_GlyphSlot _glyph, wchar_t _char, bool _antiAliased) :
	QuadMesh(false),
	NodeResource(false),
	character(_char)
{
	// get a standard plane mesh and insert the verts into the glyph
	MeshInterface * m = MeshFactory::getPlaneMesh(0.5f, false);
	insertVertices(*m);
	delete m;

	// get the glyph dimensions
	float vx = _glyph->bitmap_left;
	float vy = _glyph->bitmap_top;
	float w = _glyph->bitmap.width;
	float h = _glyph->bitmap.rows;
	advance = _glyph->advance;
	metrics = _glyph->metrics;
	
	// modify the mesh verts to match the glyph dimensions
	vertices.at(0).x = vx;
	vertices.at(0).y = vy;
	
	vertices.at(1).x = vx + w;
	vertices.at(1).y = vy;
	
	vertices.at(2).x = vx + w;
	vertices.at(2).y = vy - h;
	
	vertices.at(3).x = vx;
	vertices.at(3).y = vy - h;

	// set the scale mode
	if(!_antiAliased){
		setScaleMode(GL_NEAREST);
	}else{
		setScaleMode(GL_LINEAR);
	}
	uvEdgeMode = GL_CLAMP;
}



GlyphTexture::GlyphTexture(FT_Bitmap _glyph, bool _autoRelease) :
	Texture("", true, _autoRelease),
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
		checkForGlError(false);
		glBindTexture(GL_TEXTURE_2D, textureId);
		checkForGlError(false);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	NodeLoadable::load();
}




Font::Font(std::string _fontSrc, int _size, bool _autoRelease) :
	NodeResource(_autoRelease),
	face(nullptr),
	antiAliased(false)
{
	if(FT_New_Face(sweet::freeTypeLibrary, _fontSrc.c_str(), 0, &face) != 0) {
		Log::error("Couldn't load font: " + _fontSrc);
	}
	size = _size * (sweet::getDpi() / 75.f);
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
	GlyphTexture * res;
	loadGlyph(_char);
	auto t = textures.find(_char);
	if(t == textures.end()){
		FT_Set_Pixel_Sizes(face, 0, size);
		FT_Load_Char(face, _char, FT_LOAD_RENDER);
		GlyphTexture * tex = new GlyphTexture(face->glyph->bitmap, false);
		tex->src = _char;
		textures.insert(std::pair<wchar_t, GlyphTexture *>(_char, tex));
		res = tex;
		res->load();
	}else{
		res = t->second;
	}
	return res;
}

GlyphMesh * Font::getMeshInterfaceForChar(wchar_t _char){
	GlyphMesh * res;
	auto t = meshes.find(_char);
	if(t == meshes.end()){
		loadGlyph(_char);
		GlyphMesh * mesh = new GlyphMesh(face->glyph, _char, antiAliased);
		mesh->autoRelease = false;
		mesh->pushTexture2D(getTextureForChar(_char));
		meshes.insert(std::pair<char, GlyphMesh *>(_char, mesh));
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