#include <Font.h>
#include <Vox.h>

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
		MeshInterface * mesh = new MeshInterface(GL_QUADS, GL_STATIC_DRAW);
	
		float x = 0.0f;
		float y = 0.0f;

		float sx = 2.0f/640.0f;
		float sy = 2.0f/480.0f;

		float vx = x + face->glyph->bitmap_left * sx;
		float vy = y + face->glyph->bitmap_top * sy;
		float w = face->glyph->bitmap.width * sx;
		float h = face->glyph->bitmap.rows * sy;

		mesh->pushVert(Vertex(glm::vec3(vx, vy, -2.f), glm::vec2(0.f, 0.f)));
		mesh->pushVert(Vertex(glm::vec3(vx + w, vy, -2.f), glm::vec2(1.f, 0.f)));
		mesh->pushVert(Vertex(glm::vec3(vx + w, vy - h, -2.f), glm::vec2(1.f, 1.f)));
		mesh->pushVert(Vertex(glm::vec3(vx, vy-h, -2.f), glm::vec2(0.f, 1.f)));

		mesh->pushTexture2D(getTextureForChar(_char));
		
		meshes[_char] = mesh;
	}
	return meshes.at(_char);
}