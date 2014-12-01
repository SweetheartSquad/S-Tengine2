#pragma once

#include "VoxelMesh.h"
#include "RenderOptions.h"
#include "MatrixStack.h"

VoxelMesh::VoxelMesh(GLenum _drawMode):
	MeshInterface(GL_POINTS, _drawMode),
	resolution(0.8)
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

void VoxelMesh::configureExtras(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	glm::mat4 vp = _matrixStack->projectionMatrix * _matrixStack->viewMatrix;
	glm::mat4 m = _matrixStack->currentModelMatrix;
	GLuint vpUniformLocation = glGetUniformLocation(_renderStack->shader->getProgramId(), "VP");
	GLuint mUniformLocation =  glGetUniformLocation(_renderStack->shader->getProgramId(), "M");
	GLuint resolutionUniformLocation = glGetUniformLocation(_renderStack->shader->getProgramId(), "resolution");
	glUniformMatrix4fv(vpUniformLocation, 1, GL_FALSE, &vp[0][0]);
	glUniformMatrix4fv(mUniformLocation, 1, GL_FALSE, &m[0][0]);
	glUniform1f(resolutionUniformLocation, resolution);
}

void VoxelMesh::pushPoint(GLint _vertex){
	indices.push_back(_vertex);
}
