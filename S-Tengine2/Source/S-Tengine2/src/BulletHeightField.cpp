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

TriMesh * BulletHeightFieldShape::getMesh(bool _tileUvs){
	TriMesh * res = new TriMesh(true);

	// copy the vertices into the mesh while also setting up the UVs
	for(unsigned long int y = 0; y < heightMap->height; ++y){
		for(unsigned long int x = 0; x < heightMap->width; ++x){
			// copy verts
			btVector3 v1;
			getVertex(x, y, v1);
			Vertex v2(v1.x(), v1.y(), v1.z());
			v2.u = x;
			v2.v = y;
			if(!_tileUvs){
				v2.u /= heightMap->width;
				v2.v /= heightMap->height;
			}
			res->vertices.push_back(v2);
		}
	}
	
	// connect the verts to make triangles and set the normals on each set of 4 as the average of both triangle face normals
	for(unsigned long int i = 0; i < res->vertices.size() - heightMap->width - 1; ++i){
		if((i+1) % heightMap->width == 0){
			continue;
		}
		
		unsigned long int
			v1 = i,
			v2 = i+1,
			v3 = i+heightMap->width,
			v4 = v3+1;
			
		res->pushTri(v1, v2, v3);
		res->pushTri(v3, v2, v4);
		
		// calculate the normal as an average of both face normals
		glm::vec3 n = ( res->calcNormal(v1, v2, v3) + res->calcNormal(v3, v2, v4) ) * 0.5f;
		
		res->setNormal(i, n.x, n.y, n.z);
		res->setNormal(i+1, n.x, n.y, n.z);
		res->setNormal(i+heightMap->width, n.x, n.y, n.z);
		res->setNormal(i+heightMap->width+1, n.x, n.y, n.z);
	}

	return res;
}

BulletHeightField::BulletHeightField(BulletWorld * _world, Texture * _heightMap, Shader * _shader, bool _tileUvs, glm::vec3 _scale, unsigned long int _upAxis) :
	BulletMeshEntity(_world, new TriMesh(true), _shader),
	heightMap(_heightMap),
	tileUvs(_tileUvs)
{
	++heightMap->referenceCount;

	setColliderAsHeightMap(_heightMap, _scale, _upAxis);
	updateMesh();
}

BulletHeightField::~BulletHeightField(){
	heightMap->decrementAndDelete();
}


void BulletHeightField::updateMesh(){
	// clear out any existing verts
	mesh->vertices.clear();
	mesh->indices.clear();

	// copy in the new verts
	BulletHeightFieldShape * s = dynamic_cast<BulletHeightFieldShape *>(shape);
	TriMesh * temp = s->getMesh(tileUvs);
	mesh->insertVertices(temp);
	delete temp;
}