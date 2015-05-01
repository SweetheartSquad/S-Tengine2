#pragma once

#include "MeshInterface.h"
#include <map>

class SpriteSheetAnimation;

class SpriteMesh : public MeshInterface{
public:

	Texture * animatedTexture;

	explicit SpriteMesh(GLenum _drawMode);
	~SpriteMesh();
};