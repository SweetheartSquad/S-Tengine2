#pragma once 

#include "MeshEntity.h"
#include "MeshInterface.h"

class Sprite : public MeshEntity{
public:
	
	explicit Sprite(Shader * _shader = nullptr, Transform * _transform = new Transform());
	virtual ~Sprite();

	Vertex * getTopLeft();
	Vertex * getTopRight();
	Vertex * getBottomLeft();
	Vertex * getBottomRight();

	void setUvs(float _topLeftU, float _topLeftV, float _topRightU, float _topRightV, 
		float _bottomLeftU, float _bottomLeftV, float _bottomRightU, float _bottomRightV);
};

class SpriteMesh : public MeshInterface{
public:
	explicit SpriteMesh(GLenum _drawMode);
	virtual ~SpriteMesh();
};