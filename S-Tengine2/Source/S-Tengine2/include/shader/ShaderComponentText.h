#pragma once

#include "ShaderComponent.h"
#include <glm/detail/type_vec4.hpp>

class ShaderComponentText : public ShaderComponent{
private:
	GLint texNumLoc, texColLoc, texSamLoc;
	unsigned long int numTextures;

public:

	glm::vec4 color;
	
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
	
	glm::vec4 getColor() const;
	void setColor(glm::vec4 _color);
};
