#pragma once

#include <shader\ShaderComponent.h>

class ShaderComponentToon : public ShaderComponent {
public:	
	ShaderComponentToon(Shader * _shader, int _levels);
	~ShaderComponentToon();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	void setLevels(int _levels);
	int getLevels() const;

private:
	int levels;
	GLint levelsLoc;
};