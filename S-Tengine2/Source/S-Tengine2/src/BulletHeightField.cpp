#pragma once

#include <BulletHeightField.h>
#include <Texture.h>
#include <TextureUtils.h>
#include <MeshInterface.h>

BulletHeightFieldShape::BulletHeightFieldShape(Texture * _heightMap, glm::vec3 _scale, unsigned long int _upAxis) :
	heightMap(_heightMap),
	btHeightfieldTerrainShape(_heightMap->width, _heightMap->height, _heightMap->data, (_upAxis == 0 ? _scale.x : (_upAxis == 1 ? _scale.y : _scale.z)), sweet::TextureUtils::getMin(_heightMap), sweet::TextureUtils::getMax(_heightMap), _upAxis, PHY_ScalarType::PHY_UCHAR, false)
{
	++heightMap->referenceCount;
	setLocalScaling(btVector3(_upAxis == 0 ? 1 : _scale.x, _upAxis == 1 ? 1 : _scale.y, _upAxis == 2 ? 1 : _scale.z));
}

BulletHeightFieldShape::~BulletHeightFieldShape(){
	heightMap->decrementAndDelete();
}

TriMesh * BulletHeightFieldShape::getMesh(){
	TriMesh * res = new TriMesh(true);
	for(unsigned long int x = 1; x < heightMap->width; ++x){
		for(unsigned long int y = 1; y < heightMap->height; ++y){
			// copy verts
			btVector3 v;
			getVertex(x, y, v);
			res->vertices.push_back(Vertex(v.x(), v.y(), v.z()));
			getVertex(x-1, y, v);
			res->vertices.push_back(Vertex(v.x(), v.y(), v.z()));
			getVertex(x-1, y-1, v);
			res->vertices.push_back(Vertex(v.x(), v.y(), v.z()));
			getVertex(x, y-1, v);
			res->vertices.push_back(Vertex(v.x(), v.y(), v.z()));
			
			// set uvs
			res->setUV(res->vertices.size() - 1, (float)x/heightMap->width, (float)(y-1)/heightMap->height);
			res->setUV(res->vertices.size() - 2, (float)(x-1)/heightMap->width, (float)(y-1)/heightMap->height);
			res->setUV(res->vertices.size() - 3, (float)(x-1)/heightMap->width, (float)y/heightMap->height);
			res->setUV(res->vertices.size() - 4, (float)x/heightMap->width, (float)y/heightMap->height);
			
			// connect verts
			res->pushTri(res->vertices.size() - 4, res->vertices.size() - 3, res->vertices.size() - 2);
			res->pushTri(res->vertices.size() - 4, res->vertices.size() - 2, res->vertices.size() - 1);
		}
	}
	return res;
}

BulletHeightField::BulletHeightField(BulletWorld * _world, Texture * _heightMap, Shader * _shader, glm::vec3 _scale, unsigned long int _upAxis) :
	BulletMeshEntity(_world, new TriMesh(true), _shader),
	heightMap(_heightMap)
{
	++heightMap->referenceCount;

	setColliderAsHeightMap(_heightMap, _scale, _upAxis);

	BulletHeightFieldShape * s = dynamic_cast<BulletHeightFieldShape *>(shape);
	TriMesh * temp = s->getMesh();
	mesh->insertVertices(temp);
	delete temp;
}

BulletHeightField::~BulletHeightField(){
	heightMap->decrementAndDelete();
}
