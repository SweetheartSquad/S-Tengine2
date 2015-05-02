#pragma once

#include <Font.h>
#include <Vox.h>
#include <MeshFactory.h>

Font::Font(std::string _fontSrc, int _size):
	NodeLoadable()
{
	face = nullptr;
	if(FT_New_Face(vox::freeTypeLibrary, _fontSrc.c_str(), 0, &face) != 0) {
		std::cerr << "Couldn't load font " << _fontSrc;
	}
	size = _size;
}

Font::~Font(){
	for(auto c : textures){
		delete c.second;
	}
	for(auto c : meshes){
		delete c.second;
	}
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

GlyphTexture::GlyphTexture(FT_Bitmap _glyph, bool _storeData, bool _autoRelease) :
	Texture(_storeData, _autoRelease),
	NodeResource(_autoRelease)
{
	width = _glyph.width;
	height = _glyph.rows;
	data = _glyph.buffer;
	channels = 2;
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

		if(!storeData){
			data = nullptr;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	
	NodeLoadable::load();
}


GlyphTexture * Font::getTextureForChar(char _char){
	if(textures.count(_char) < 1){
		FT_Set_Pixel_Sizes(face, 0, size);
		FT_Load_Char(face, _char, FT_LOAD_RENDER);
		GlyphTexture * tex = new GlyphTexture(face->glyph->bitmap, true, false);
		textures.insert(std::pair<char, GlyphTexture *>(_char, tex));
		tex->unload();
		tex->load();
	}
	return textures.at(_char);
}

MeshInterface* Font::getMeshInterfaceForChar(char _char){
	auto t = meshes.find(_char);
	MeshInterface * res;
	if(t == meshes.end()){
		loadGlyph(_char);

		MeshInterface * mesh = MeshFactory::getPlaneMesh();
		
		mesh->pushTexture2D(getTextureForChar(_char));

		float vx = face->glyph->bitmap_left;
		float vy = face->glyph->bitmap_top;
		float w = face->glyph->bitmap.width;
		float h = face->glyph->bitmap.rows;
		
		mesh->vertices.at(0).x = vx;
		mesh->vertices.at(0).y = vy;

		mesh->vertices.at(1).x = vx + w;
		mesh->vertices.at(1).y = vy;

		mesh->vertices.at(2).x = vx + w;
		mesh->vertices.at(2).y = vy - h;

		mesh->vertices.at(3).x = vx;
		mesh->vertices.at(3).y = vy - h;

		mesh->dirty = true;
		//mesh->loaded = false;
		//mesh->unload();
		//mesh->load();
		mesh->referenceCount++;

		meshes.insert(std::pair<char, MeshInterface *>(_char, mesh));
		res = mesh;
	}else{
		res = t->second;
	}
	return res;
}

glm::vec2 Font::getGlyphWidthHeight(char _char){
	loadGlyph(_char);
	return glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
}

glm::vec2 Font::getGlyphXY(char _char){
	loadGlyph(_char);
	return glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
}

void Font::loadGlyph(char _char){
	FT_Set_Pixel_Sizes(face, 0, size);
	FT_Load_Char(face, _char, FT_LOAD_RENDER);
}