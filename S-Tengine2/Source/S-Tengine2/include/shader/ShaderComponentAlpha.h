#pragma once 

#include <shader\ShaderComponent.h>

class ShaderComponentAlpha : public ShaderComponent{
private:
	float alpha;
	GLint alphaLoc;
public:
	float getAlpha();
	void setAlpha(float _alpha);

	// all values are given as 0 - 1, a *= alpha
	ShaderComponentAlpha(ComponentShaderBase * _shader, float _alpha = 1.f);
	~ShaderComponentAlpha();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};