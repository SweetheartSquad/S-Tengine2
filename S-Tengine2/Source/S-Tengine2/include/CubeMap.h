#pragma once

#include <MeshEntity.h>
#include <CubeMapTexture.h>
#include <shader/ShaderComponentCubeMap.h>

class CubeMap : public MeshEntity{
public:
	ComponentShaderBase * cubemapShader;
	CubeMapTexture * cubemapTexture;
	CubeMap(std::string _src);

	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
};