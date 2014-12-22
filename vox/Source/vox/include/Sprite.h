#pragma once 

#include "MeshEntity.h"
#include "MeshInterface.h"

class Sprite : public MeshEntity{
public:
	
	explicit Sprite(Shader * _shader = nullptr, Transform * _transform = new Transform());
	virtual ~Sprite();
};

class SpriteMesh : public MeshInterface{
public:
	explicit SpriteMesh(GLenum _drawMode);
	virtual ~SpriteMesh();
};