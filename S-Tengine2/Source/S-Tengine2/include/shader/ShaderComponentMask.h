#pragma once

#include <shader/ShaderComponent.h>
#include <GL/glew.h>

/**
* Simple shader component for rendering based on a mask texture
*/
class ShaderComponentMask : public ShaderComponent {
public:
	explicit ShaderComponentMask(Shader * _shader);

	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;

	void setMaskTextureId(GLuint _id);

private:

	GLuint maskTextureId; 
};