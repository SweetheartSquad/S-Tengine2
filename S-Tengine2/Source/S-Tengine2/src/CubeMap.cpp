#pragma once

#include <CubeMap.h>
#include <shader/ComponentShaderBase.h>
#include <MeshInterface.h>
#include <MeshFactory.h>

CubeMap::CubeMap(std::string _srcPrefix, std::string _srcSuffix) :
	cubemapTexture(new CubeMapTexture(_srcPrefix, _srcSuffix, true, true, false)),
	cubemapShader(new ComponentShaderBase(true)),
	MeshEntity(MeshFactory::getCubeMesh(1.f), nullptr)
{
	cubemapShader->addComponent(new ShaderComponentCubeMap(cubemapShader));
	cubemapShader->compileShader();
	
	cubemapTexture->load();

	setShader(cubemapShader, true);
	mesh->pushTexture2D(cubemapTexture);

	mesh->setScaleMode(GL_LINEAR);
	mesh->uvEdgeMode = GL_CLAMP_TO_EDGE;
	cubemapShader->incrementReferenceCount();
}

CubeMap::~CubeMap(){
	cubemapShader->decrementAndDelete();
}

void CubeMap::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption){
	cubemapShader->makeDirty(); // always dirty because we need to update the view matrix
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture->textureId);
	MeshEntity::render(_matrixStack, _renderOption);
}