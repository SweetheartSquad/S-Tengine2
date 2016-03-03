#pragma once

#include <MeshEntity.h>
#include <CubeMapTexture.h>
#include <shader/ShaderComponentCubeMap.h>

class CubeMap : public MeshEntity{
public:
	ComponentShaderBase * cubemapShader;
	CubeMapTexture * cubemapTexture;

	// _srxPrefix is the directory which leads to the cubemap and _srcSuffix is the file format
	// (i.e. if the textures are "cubemap/posx.png", "cubemap/posy.png", etc. the _srcPrefix would be "cubemap" and _srcSuffix would be "png")
	CubeMap(std::string _srcPrefix, std::string _srcSuffix);
	~CubeMap();

	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
};