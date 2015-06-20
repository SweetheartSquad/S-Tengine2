#pragma once

#include <shader/Shader.h>

/**
* Simple shader for rendering normals
*/
class NormalsShader : public Shader {
public:	
	explicit NormalsShader(bool _autoRelease);

	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

private:
	std::string getVertString();
	std::string getFragString();
};