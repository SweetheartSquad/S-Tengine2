#pragma once

#include <Sprite.h>

class Billboard : public Sprite{
public:
	static Shader * billboardShader;

	Billboard();
	~Billboard();
};