#pragma once

#include "ShaderComponent.h"
#include <glm/detail/type_vec3.hpp>

class ShaderComponentText : public ShaderComponent{
private:
	GLint texNumLoc, texColLoc, texSamLoc;
	unsigned long int numTextures;

public:

	glm::vec3 color;
	
	ShaderComponentText(ComponentShaderBase * _shader);
	~ShaderComponentText() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	virtual void load() override;
	virtual void unload() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	
	glm::vec3 getColor();
	void setColor(glm::vec3 _color);
};