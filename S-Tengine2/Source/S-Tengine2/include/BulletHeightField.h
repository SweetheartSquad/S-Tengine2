#pragma once

#include <BulletMeshEntity.h>
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>

class Texture;
class QuadMesh;

// since a height field doesn't copy the data used to create it,
// we need to make sure that we don't accidentally delete the
// texture data while it's in use
//
// NOTE: maybe it'd be better if this class copied the data instead? then we woudn't have to worry about it
class BulletHeightFieldShape : public btHeightfieldTerrainShape{
private:
	Texture * heightMap;
public:
	// _upAxis = 0, 1, or 2, indicating x, y, and z axis respectively
	BulletHeightFieldShape(Texture * _heightMap, glm::vec3 _scale = glm::vec3(1.f), unsigned long int _upAxis = 1);
	~BulletHeightFieldShape();
	
	// _tileUvs = if true, sets UVs such that each square on the height map is 0-1; if false sets UVs such that the entire height map is 0-1
	TriMesh * getMesh(bool _tileUvs);
};

class BulletHeightField : public BulletMeshEntity{
private:
	Texture * heightMap;
public:
	// NOTE: a BulletHeightField calls setColliderAsHeightMap in its constructor, so you don't need to construct the shape yourself
	// _tileUvs = if true, sets UVs such that each square on the height map is 0-1; if false sets UVs such that the entire height map is 0-1
	// _upAxis = 0, 1, or 2, indicating x, y, and z axis respectively
	BulletHeightField(BulletWorld * _world, Texture * _heightMap, Shader * _shader, bool _tileUvs = false, glm::vec3 _scale = glm::vec3(1.f), unsigned long int _upAxis = 1);
	~BulletHeightField();
};