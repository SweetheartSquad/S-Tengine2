#pragma once

#include "VoxelMesh.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "NodeRenderable.h"

VoxelMesh::VoxelMesh(GLenum _drawMode):
	MeshInterface(GL_POINTS, _drawMode),
	NodeRenderable(),
	resolution(0.15)
{
}

VoxelMesh::VoxelMesh(MeshInterface* _mesh):
	MeshInterface(GL_POINTS, _mesh->drawMode),
	resolution(0.15){
	indices  = _mesh->indices;
	vertices = _mesh->vertices;
	unload();
	load();
}

VoxelMesh::~VoxelMesh(){
}

void VoxelMesh::pushPoint(GLint _vertex){
	indices.push_back(_vertex);
}
