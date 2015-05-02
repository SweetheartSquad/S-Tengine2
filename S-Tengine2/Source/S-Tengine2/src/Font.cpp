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
	for(auto c : textures){
		c.second->load();
	}
	for(auto c : meshes){
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

Texture * Font::getTextureForChar(char _char){
	if(!textures.count(_char)){
		FT_Set_Pixel_Sizes(face, 0, size);
		FT_GlyphSlot glyph = face->glyph;
		FT_Load_Char(face, _char, FT_LOAD_RENDER);
		textures[_char] = new Texture(glyph->bitmap, true, false);	
	}
	return textures.at(_char);
}

MeshInterface* Font::getMeshInterfaceForChar(char _char){
	if(!meshes.count(_char)){
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
		mesh->vertices.at(3).y = vy-h;

		mesh->dirty = true;

		mesh->unload();
		mesh->load();
		
		meshes[_char] = mesh;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	return meshes.at(_char);
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