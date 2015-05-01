#pragma once

#include <string>
#include <map>

#include <Texture.h>
#include <node/NodeLoadable.h>
#include "MeshInterface.h"

class Font : public NodeLoadable{
public:	
	
	FT_Face face;
	std::map<char, Texture *> textures;
	std::map<char, MeshInterface *> meshes;

	explicit Font(std::string _fontSrc, int size);
	~Font();
	
	Texture * getTextureForChar(char _char);
	MeshInterface * getMeshInterfaceForChar(char _char);

	void load() override;
	void unload() override;

private:
	int size;
};